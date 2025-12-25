//===--- AvoidTypePunningCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTypePunningCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidTypePunningCheck::registerMatchers(MatchFinder *Finder) {
  // Match dereferencing of reinterpret_cast pointer results
  // Pattern: *(reinterpret_cast<T*>(expr))
  Finder->addMatcher(
      unaryOperator(hasOperatorName("*"),
                    hasUnaryOperand(expr(hasType(pointerType()),
                                         anyOf(cxxReinterpretCastExpr().bind(
                                                   "reinterpret_cast"),
                                               cStyleCastExpr().bind("c_cast")))
                                        .bind("cast_expr")))
          .bind("deref"),
      this);

  // Match member access through reinterpret_cast pointer
  // Pattern: reinterpret_cast<T*>(expr)->member
  Finder->addMatcher(
      memberExpr(
          hasObjectExpression(
              expr(hasType(pointerType()),
                   anyOf(cxxReinterpretCastExpr().bind("reinterpret_cast"),
                         cStyleCastExpr().bind("c_cast")))
                  .bind("cast_expr")))
          .bind("member_access"),
      this);

  // Match array subscript through reinterpret_cast pointer
  // Pattern: reinterpret_cast<T*>(expr)[index]
  Finder->addMatcher(
      arraySubscriptExpr(
          hasBase(expr(hasType(pointerType()),
                       anyOf(cxxReinterpretCastExpr().bind("reinterpret_cast"),
                             cStyleCastExpr().bind("c_cast")))
                      .bind("cast_expr")))
          .bind("subscript"),
      this);

  // Match dereferencing of C-style cast pointer results for cross-type access
  // This catches patterns like: *(float*)&x where x is int
  Finder->addMatcher(
      unaryOperator(hasOperatorName("&"),
                    hasUnaryOperand(declRefExpr().bind("addr_of_expr")))
          .bind("address_of"),
      this);
}

bool AvoidTypePunningCheck::isCharacterType(QualType Type) const {
  Type = Type.getCanonicalType().getUnqualifiedType();
  return Type->isCharType() ||
         Type->isSpecificBuiltinType(BuiltinType::SChar) ||
         Type->isSpecificBuiltinType(BuiltinType::UChar);
}

bool AvoidTypePunningCheck::areTypesCompatible(QualType FromType,
                                               QualType ToType,
                                               const ASTContext &Ctx) const {
  // Get canonical types for comparison
  QualType FromCanon = FromType.getCanonicalType().getUnqualifiedType();
  QualType ToCanon = ToType.getCanonicalType().getUnqualifiedType();

  // Same types are compatible
  if (FromCanon == ToCanon)
    return true;

  // void* is compatible with any pointer
  if (ToCanon->isVoidType() || FromCanon->isVoidType())
    return true;

  // Character types (char, signed char, unsigned char) can alias anything
  if (isCharacterType(ToCanon) || isCharacterType(FromCanon))
    return true;

  // Check for pointer types
  if (const auto *FromPtr = FromCanon->getAs<PointerType>()) {
    if (const auto *ToPtr = ToCanon->getAs<PointerType>()) {
      QualType FromPointee = FromPtr->getPointeeType().getCanonicalType();
      QualType ToPointee = ToPtr->getPointeeType().getCanonicalType();

      // Check for derived/base relationships
      if (FromPointee->isRecordType() && ToPointee->isRecordType()) {
        const auto *FromRecord = FromPointee->getAsCXXRecordDecl();
        const auto *ToRecord = ToPointee->getAsCXXRecordDecl();
        if (FromRecord && ToRecord) {
          // Check if one is a base of the other
          if (FromRecord->isDerivedFrom(ToRecord) ||
              ToRecord->isDerivedFrom(FromRecord))
            return true;
        }
      }
    }
  }

  // Check for reference types
  if (const auto *FromRef = FromCanon->getAs<ReferenceType>()) {
    if (const auto *ToRef = ToCanon->getAs<ReferenceType>()) {
      return areTypesCompatible(FromRef->getPointeeType(),
                                ToRef->getPointeeType(), Ctx);
    }
  }

  return false;
}

void AvoidTypePunningCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Deref = Result.Nodes.getNodeAs<UnaryOperator>("deref");
  const auto *MemberAccess =
      Result.Nodes.getNodeAs<MemberExpr>("member_access");
  const auto *Subscript =
      Result.Nodes.getNodeAs<ArraySubscriptExpr>("subscript");
  const auto *ReinterpretCast =
      Result.Nodes.getNodeAs<CXXReinterpretCastExpr>("reinterpret_cast");
  const auto *CStyleCast = Result.Nodes.getNodeAs<CStyleCastExpr>("c_cast");
  const auto *TheCastExpr = Result.Nodes.getNodeAs<Expr>("cast_expr");
  (void)TheCastExpr; // May be unused

  // Determine which pattern we matched
  const Expr *AccessExpr = nullptr;
  StringRef AccessKind;

  if (Deref) {
    AccessExpr = Deref;
    AccessKind = "dereferencing";
  } else if (MemberAccess) {
    AccessExpr = MemberAccess;
    AccessKind = "member access through";
  } else if (Subscript) {
    AccessExpr = Subscript;
    AccessKind = "array subscript through";
  } else {
    return;
  }

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(AccessExpr->getBeginLoc()))
    return;

  // Skip if in macro expansion (to avoid noise from system macros)
  if (AccessExpr->getBeginLoc().isMacroID())
    return;

  // We need either a reinterpret_cast or a C-style cast that changes type
  if (!ReinterpretCast && !CStyleCast)
    return;

  const CastExpr *Cast = ReinterpretCast
                             ? static_cast<const CastExpr *>(ReinterpretCast)
                             : static_cast<const CastExpr *>(CStyleCast);

  if (!Cast)
    return;

  // Get the source and destination types
  QualType ToType = Cast->getType();
  QualType FromType = Cast->getSubExpr()->getType();

  // For pointers, extract the pointee types for comparison
  QualType ToPointee;
  QualType FromPointee;

  if (const auto *ToPtr = ToType->getAs<PointerType>()) {
    ToPointee = ToPtr->getPointeeType();
  } else {
    return; // Not a pointer cast, not relevant for type-punning
  }

  if (const auto *FromPtr = FromType->getAs<PointerType>()) {
    FromPointee = FromPtr->getPointeeType();
  } else {
    // Casting from non-pointer (e.g., integer to pointer)
    // This is suspicious but we focus on pointer-to-pointer casts
    return;
  }

  // Check if the types are compatible
  if (areTypesCompatible(FromPointee, ToPointee, *Result.Context))
    return;

  // Report the violation
  SourceLocation DiagLoc = AccessExpr->getBeginLoc();
  if (ReinterpretCast) {
    diag(DiagLoc,
         "type-punning through %0 reinterpret_cast from %1 to %2 results in "
         "undefined behavior")
        << AccessKind << FromPointee << ToPointee;
  } else {
    diag(DiagLoc,
         "type-punning through %0 C-style cast from %1 to %2 results in "
         "undefined behavior")
        << AccessKind << FromPointee << ToPointee;
  }

  diag(Cast->getBeginLoc(), "cast occurs here", DiagnosticIDs::Note);
}

} // namespace clang::tidy::automotive::cpp23

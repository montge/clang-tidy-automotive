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
  // Common matcher for cast expressions (reinterpret_cast or C-style cast)
  auto castMatcher = expr(ignoringParenImpCasts(
      anyOf(cxxReinterpretCastExpr().bind("reinterpret_cast"),
            cStyleCastExpr().bind("c_cast"))));

  // Match dereferencing of cast pointer results
  // Pattern: *(reinterpret_cast<T*>(expr)) or *((T*)expr)
  Finder->addMatcher(
      unaryOperator(hasOperatorName("*"),
                    hasUnaryOperand(expr(hasType(pointerType()), castMatcher)
                                        .bind("cast_expr")))
          .bind("deref"),
      this);

  // Match member access through cast pointer
  // Pattern: reinterpret_cast<T*>(expr)->member or ((T*)expr)->member
  Finder->addMatcher(
      memberExpr(isArrow(),
                 hasObjectExpression(
                     expr(hasType(pointerType()),
                          ignoringParenImpCasts(anyOf(
                              cxxReinterpretCastExpr().bind("reinterpret_cast"),
                              cStyleCastExpr().bind("c_cast"))))
                         .bind("cast_expr")))
          .bind("member_access"),
      this);

  // Match array subscript through cast pointer
  // Pattern: reinterpret_cast<T*>(expr)[index] or ((T*)expr)[index]
  Finder->addMatcher(
      arraySubscriptExpr(
          hasBase(expr(hasType(pointerType()),
                       ignoringParenImpCasts(anyOf(
                           cxxReinterpretCastExpr().bind("reinterpret_cast"),
                           cStyleCastExpr().bind("c_cast"))))
                      .bind("cast_expr")))
          .bind("subscript"),
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
  bool IsIntegerToPointerCast = false;

  if (const auto *ToPtr = ToType->getAs<PointerType>()) {
    ToPointee = ToPtr->getPointeeType();
  } else {
    return; // Not a pointer cast, not relevant for type-punning
  }

  if (const auto *FromPtr = FromType->getAs<PointerType>()) {
    FromPointee = FromPtr->getPointeeType();
  } else if (FromType->isIntegerType()) {
    // Casting from integer to pointer (e.g., *(int*)addr)
    // This is type-punning: we're treating an integer as a memory address
    // and then accessing it as a specific type
    IsIntegerToPointerCast = true;
    FromPointee = FromType; // Use the integer type as "from" type
  } else {
    return;
  }

  // For integer-to-pointer casts, always warn (except for void* or char* targets)
  if (IsIntegerToPointerCast) {
    // Allow casting to void* or char* (common for low-level memory access)
    if (ToPointee->isVoidType() || isCharacterType(ToPointee))
      return;
  } else {
    // For pointer-to-pointer casts, check if the types are compatible
    if (areTypesCompatible(FromPointee, ToPointee, *Result.Context))
      return;
  }

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

  // For reinterpret_cast, point to the keyword; for C-style cast, point to the
  // opening paren
  SourceLocation NoteLoc = Cast->getBeginLoc();
  diag(NoteLoc, "cast occurs here", DiagnosticIDs::Note);
}

} // namespace clang::tidy::automotive::cpp23

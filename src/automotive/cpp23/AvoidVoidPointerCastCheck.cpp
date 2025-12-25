//===--- AvoidVoidPointerCastCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidVoidPointerCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidVoidPointerCastCheck::registerMatchers(MatchFinder *Finder) {
  // Match C++ casts (static_cast, reinterpret_cast, const_cast) from void* to
  // typed pointer
  Finder->addMatcher(
      cxxStaticCastExpr(
          hasDestinationType(pointerType(unless(pointee(voidType())))),
          hasSourceExpression(hasType(pointerType(pointee(voidType())))))
          .bind("static_cast"),
      this);

  Finder->addMatcher(
      cxxReinterpretCastExpr(
          hasDestinationType(pointerType(unless(pointee(voidType())))),
          hasSourceExpression(hasType(pointerType(pointee(voidType())))))
          .bind("reinterpret_cast"),
      this);

  Finder->addMatcher(
      cxxConstCastExpr(
          hasDestinationType(pointerType(unless(pointee(voidType())))),
          hasSourceExpression(hasType(pointerType(pointee(voidType())))))
          .bind("const_cast"),
      this);

  // Match C-style casts from void* to typed pointer
  Finder->addMatcher(
      cStyleCastExpr(
          hasDestinationType(pointerType(unless(pointee(voidType())))),
          hasSourceExpression(hasType(pointerType(pointee(voidType())))))
          .bind("c_style_cast"),
      this);

  // Match implicit casts from void* to typed pointer (primarily in C)
  // In C++, implicit conversion from void* is not allowed, but we check anyway
  Finder->addMatcher(
      implicitCastExpr(hasType(pointerType(unless(pointee(voidType())))),
                       has(expr(hasType(pointerType(pointee(voidType()))))),
                       unless(hasParent(explicitCastExpr())))
          .bind("implicit_cast"),
      this);
}

bool AvoidVoidPointerCastCheck::hasTypeVerification(const Expr *CastExpr,
                                                    ASTContext *Context) const {
  if (!CastExpr)
    return false;

  // Traverse up the AST to look for type verification patterns
  const auto &Parents = Context->getParents(*CastExpr);
  if (Parents.empty())
    return false;

  // Check if the cast is used in a sizeof comparison
  // Pattern: sizeof(*ptr) == sizeof(TargetType)
  for (const auto &Parent : Parents) {
    if (const auto *BinOp = Parent.get<BinaryOperator>()) {
      if (BinOp->isComparisonOp()) {
        // Check if either side involves sizeof
        const Expr *LHS = BinOp->getLHS()->IgnoreImpCasts();
        const Expr *RHS = BinOp->getRHS()->IgnoreImpCasts();

        if (isa<UnaryExprOrTypeTraitExpr>(LHS) ||
            isa<UnaryExprOrTypeTraitExpr>(RHS)) {
          return true;
        }
      }
    }

    // Check if it's in a conditional/if statement with type checks
    if (Parent.get<IfStmt>() || Parent.get<ConditionalOperator>()) {
      // Could have more sophisticated checks here for type verification
      // For now, we'll be conservative and still warn
      return false;
    }
  }

  return false;
}

void AvoidVoidPointerCastCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *StaticCast =
      Result.Nodes.getNodeAs<CXXStaticCastExpr>("static_cast");
  const auto *ReinterpretCast =
      Result.Nodes.getNodeAs<CXXReinterpretCastExpr>("reinterpret_cast");
  const auto *ConstCast =
      Result.Nodes.getNodeAs<CXXConstCastExpr>("const_cast");
  const auto *CStyleCast =
      Result.Nodes.getNodeAs<CStyleCastExpr>("c_style_cast");
  const auto *ImplicitCast =
      Result.Nodes.getNodeAs<ImplicitCastExpr>("implicit_cast");

  const CastExpr *TheCast = nullptr;
  StringRef CastKind;

  if (StaticCast) {
    TheCast = StaticCast;
    CastKind = "static_cast";
  } else if (ReinterpretCast) {
    TheCast = ReinterpretCast;
    CastKind = "reinterpret_cast";
  } else if (ConstCast) {
    TheCast = ConstCast;
    CastKind = "const_cast";
  } else if (CStyleCast) {
    TheCast = CStyleCast;
    CastKind = "C-style cast";
  } else if (ImplicitCast) {
    TheCast = ImplicitCast;
    CastKind = "implicit conversion";
  } else {
    return;
  }

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(TheCast->getBeginLoc()))
    return;

  // Skip macro expansions (to avoid noise from system macros)
  if (TheCast->getBeginLoc().isMacroID())
    return;

  // Check if there's type verification
  if (hasTypeVerification(TheCast, Result.Context))
    return;

  // Get destination type for the diagnostic
  QualType DestType = TheCast->getType();

  // Extract pointee type for better diagnostics
  QualType DestPointee;
  if (const auto *DestPtr = DestType->getAs<PointerType>()) {
    DestPointee = DestPtr->getPointeeType();
  }

  // Report the violation
  SourceLocation DiagLoc = TheCast->getBeginLoc();

  if (ImplicitCast) {
    diag(DiagLoc,
         "implicit conversion from void* to %0 is not verified and may be "
         "unsafe; use explicit cast with type verification")
        << DestPointee;
  } else if (CStyleCast) {
    diag(DiagLoc,
         "C-style cast from void* to %0 is not verified and may be unsafe; "
         "use C++ cast with type verification")
        << DestPointee;
  } else {
    diag(DiagLoc, "%0 from void* to %1 is not verified and may be unsafe; "
                  "ensure pointer validity and type compatibility")
        << CastKind << DestPointee;
  }
}

} // namespace clang::tidy::automotive::cpp23

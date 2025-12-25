//===--- GenericImplicitPointerCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericImplicitPointerCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericImplicitPointerCheck::registerMatchers(MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("genericSelection"), this);
}

void GenericImplicitPointerCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  // Get the controlling expression (the expression being type-checked)
  const Expr *ControllingExpr = GenericSelection->getControllingExpr();
  if (!ControllingExpr)
    return;

  // Check if the controlling expression has implicit pointer conversions
  if (hasImplicitPointerConversion(ControllingExpr)) {
    diag(ControllingExpr->getBeginLoc(),
         "generic selection should not depend on implicit pointer type "
         "conversion in controlling expression");
  }
}

bool GenericImplicitPointerCheck::hasImplicitPointerConversion(
    const Expr *E) const {
  if (!E)
    return false;

  // Strip parentheses to get to the actual expression
  E = E->IgnoreParens();

  // Check if this is an implicit cast expression
  if (const auto *Cast = dyn_cast<ImplicitCastExpr>(E)) {
    if (isProblematicPointerCast(Cast))
      return true;

    // Recursively check the sub-expression
    return hasImplicitPointerConversion(Cast->getSubExpr());
  }

  // Check other expression types that might contain implicit conversions
  if (const auto *UnaryOp = dyn_cast<UnaryOperator>(E)) {
    return hasImplicitPointerConversion(UnaryOp->getSubExpr());
  }

  if (const auto *BinaryOp = dyn_cast<BinaryOperator>(E)) {
    return hasImplicitPointerConversion(BinaryOp->getLHS()) ||
           hasImplicitPointerConversion(BinaryOp->getRHS());
  }

  if (const auto *ConditionalOp = dyn_cast<ConditionalOperator>(E)) {
    return hasImplicitPointerConversion(ConditionalOp->getCond()) ||
           hasImplicitPointerConversion(ConditionalOp->getTrueExpr()) ||
           hasImplicitPointerConversion(ConditionalOp->getFalseExpr());
  }

  return false;
}

bool GenericImplicitPointerCheck::isProblematicPointerCast(
    const ImplicitCastExpr *Cast) const {
  if (!Cast)
    return false;

  CastKind Kind = Cast->getCastKind();
  QualType SourceType = Cast->getSubExpr()->getType();
  QualType DestType = Cast->getType();

  // Check for array-to-pointer decay
  // This is problematic because _Generic(arr, ...) will match the decayed
  // pointer type, not the array type
  if (Kind == CK_ArrayToPointerDecay) {
    return true;
  }

  // Check for pointer conversions where both source and dest are pointers
  if (SourceType->isPointerType() && DestType->isPointerType()) {
    QualType SourcePointee = SourceType->getPointeeType();
    QualType DestPointee = DestType->getPointeeType();

    // Pointer-to-void conversions are problematic
    if (DestPointee->isVoidType() && !SourcePointee->isVoidType()) {
      return true;
    }

    // Void-to-pointer conversions
    if (SourcePointee->isVoidType() && !DestPointee->isVoidType()) {
      return true;
    }

    // Conversions between different pointer types (excluding cv-qualifiers)
    if (Kind == CK_BitCast || Kind == CK_PointerToIntegral ||
        Kind == CK_IntegralToPointer) {
      return true;
    }

    // Check for conversions that change the pointer type (not just adding
    // const)
    if (!SourcePointee.isNull() && !DestPointee.isNull()) {
      QualType UnqualSourcePointee = SourcePointee.getUnqualifiedType();
      QualType UnqualDestPointee = DestPointee.getUnqualifiedType();

      if (!UnqualSourcePointee.isNull() && !UnqualDestPointee.isNull()) {
        if (!UnqualSourcePointee->isVoidType() &&
            !UnqualDestPointee->isVoidType() &&
            UnqualSourcePointee != UnqualDestPointee) {
          return true;
        }
      }
    }
  }

  // Null pointer conversions
  if (Kind == CK_NullToPointer) {
    return true;
  }

  // Function-to-pointer decay
  if (Kind == CK_FunctionToPointerDecay) {
    return true;
  }

  return false;
}

} // namespace clang::tidy::automotive

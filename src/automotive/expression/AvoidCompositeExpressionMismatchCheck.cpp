//===--- AvoidCompositeExpressionMismatchCheck.cpp - clang-tidy -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCompositeExpressionMismatchCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidCompositeExpressionMismatchCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match assignments where RHS is an arithmetic expression
  Finder->addMatcher(
      binaryOperator(
          isAssignmentOperator(),
          hasRHS(binaryOperator(
                     anyOf(hasAnyOperatorName("+", "-", "*", "/", "%"),
                           hasAnyOperatorName("&", "|", "^", "<<", ">>")))
                     .bind("composite")))
          .bind("assign"),
      this);

  // Match casts of composite expressions
  Finder->addMatcher(
      cStyleCastExpr(
          hasSourceExpression(
              binaryOperator(
                  anyOf(hasAnyOperatorName("+", "-", "*", "/", "%"),
                        hasAnyOperatorName("&", "|", "^", "<<", ">>")))
                  .bind("castcomposite")))
          .bind("cast"),
      this);
}

void AvoidCompositeExpressionMismatchCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Rule 10.6: Composite expression assigned to wider type
  if (const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assign")) {
    if (Result.SourceManager->isInSystemHeader(Assign->getOperatorLoc()))
      return;

    const auto *Composite = Result.Nodes.getNodeAs<BinaryOperator>("composite");
    if (!Composite)
      return;

    unsigned LHSWidth =
        getTypeWidth(Assign->getLHS()->getType(), *Result.Context);
    unsigned RHSWidth = getTypeWidth(Composite->getType(), *Result.Context);

    if (LHSWidth > RHSWidth) {
      diag(Assign->getOperatorLoc(),
           "composite expression with narrower type assigned to object with "
           "wider type; computation may lose precision before widening");
    }
    return;
  }

  // Rule 10.8: Cast of composite expression
  if (const auto *Cast = Result.Nodes.getNodeAs<CStyleCastExpr>("cast")) {
    if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
      return;

    const auto *Composite =
        Result.Nodes.getNodeAs<BinaryOperator>("castcomposite");
    if (!Composite)
      return;

    QualType FromType = Composite->getType();
    QualType ToType = Cast->getType();

    unsigned FromWidth = getTypeWidth(FromType, *Result.Context);
    unsigned ToWidth = getTypeWidth(ToType, *Result.Context);

    if (ToWidth > FromWidth) {
      diag(Cast->getBeginLoc(),
           "cast of composite expression to wider type; computation may lose "
           "precision before cast");
    }
  }
}

unsigned AvoidCompositeExpressionMismatchCheck::getTypeWidth(
    QualType Type, const ASTContext &Ctx) const {
  Type = Type.getCanonicalType();
  if (Type->isIntegerType() || Type->isFloatingType())
    return Ctx.getTypeSize(Type);
  return 0;
}

bool AvoidCompositeExpressionMismatchCheck::isCompositeExpression(
    const Expr *E) const {
  if (const auto *BinOp = dyn_cast<BinaryOperator>(E))
    return BinOp->isAdditiveOp() || BinOp->isMultiplicativeOp() ||
           BinOp->isBitwiseOp() || BinOp->isShiftOp();
  return false;
}

} // namespace clang::tidy::automotive

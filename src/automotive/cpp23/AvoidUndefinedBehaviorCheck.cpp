//===--- AvoidUndefinedBehaviorCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUndefinedBehaviorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidUndefinedBehaviorCheck::registerMatchers(MatchFinder *Finder) {
  // Match division and modulo operations
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("/"), hasOperatorName("%")))
          .bind("division"),
      this);

  // Match shift operations
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("<<"), hasOperatorName(">>")))
          .bind("shift"),
      this);

  // Match null pointer dereference (unary dereference of nullptr)
  Finder->addMatcher(
      unaryOperator(hasOperatorName("*"),
                    hasUnaryOperand(ignoringParenImpCasts(
                        cxxNullPtrLiteralExpr())))
          .bind("null_deref"),
      this);

  // Match array subscript with null pointer
  Finder->addMatcher(
      arraySubscriptExpr(hasBase(ignoringParenImpCasts(
                             cxxNullPtrLiteralExpr())))
          .bind("null_subscript"),
      this);
}

void AvoidUndefinedBehaviorCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Check for division/modulo by zero
  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("division")) {
    checkDivisionByZero(BinOp, Result.Context);
    return;
  }

  // Check for invalid shift amounts
  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("shift")) {
    checkShiftAmount(BinOp, Result.Context);
    return;
  }

  // Check for null pointer dereference
  if (const auto *UnOp = Result.Nodes.getNodeAs<UnaryOperator>("null_deref")) {
    diag(UnOp->getOperatorLoc(),
         "dereferencing null pointer is undefined behavior "
         "(MISRA C++:2023 Rule 0.3)");
    return;
  }

  // Check for null pointer array subscript
  if (const auto *ArrSub =
          Result.Nodes.getNodeAs<ArraySubscriptExpr>("null_subscript")) {
    diag(ArrSub->getBeginLoc(),
         "array subscript on null pointer is undefined behavior "
         "(MISRA C++:2023 Rule 0.3)");
    return;
  }
}

void AvoidUndefinedBehaviorCheck::checkDivisionByZero(
    const BinaryOperator *BinOp, ASTContext *Context) {

  const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

  // Try to evaluate the RHS as a constant
  Expr::EvalResult Result;
  if (!RHS->EvaluateAsRValue(Result, *Context))
    return;

  const APValue &Val = Result.Val;

  // Check if the divisor is zero
  bool isZero = false;
  if (Val.isInt()) {
    isZero = Val.getInt().isZero();
  } else if (Val.isFloat()) {
    isZero = Val.getFloat().isZero();
  } else {
    return;
  }

  if (isZero) {
    diag(BinOp->getOperatorLoc(),
         "division by zero is undefined behavior "
         "(MISRA C++:2023 Rule 0.3)");
  }
}

void AvoidUndefinedBehaviorCheck::checkShiftAmount(
    const BinaryOperator *BinOp, ASTContext *Context) {

  const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
  const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

  // Try to evaluate the shift amount
  Expr::EvalResult ShiftResult;
  if (!RHS->EvaluateAsRValue(ShiftResult, *Context))
    return;

  if (!ShiftResult.Val.isInt())
    return;

  const llvm::APSInt &ShiftAmount = ShiftResult.Val.getInt();

  // Check for negative shift amount
  if (ShiftAmount.isNegative()) {
    diag(BinOp->getOperatorLoc(),
         "shift by negative amount is undefined behavior "
         "(MISRA C++:2023 Rule 0.3)");
    return;
  }

  // Get the bit width of the left operand type
  QualType LHSType = LHS->getType();
  if (!LHSType->isIntegerType())
    return;

  uint64_t TypeWidth = Context->getTypeSize(LHSType);

  // Check if shift amount is >= bit width
  if (ShiftAmount.getZExtValue() >= TypeWidth) {
    diag(BinOp->getOperatorLoc(),
         "shift amount >= width of type is undefined behavior "
         "(MISRA C++:2023 Rule 0.3)");
  }
}

} // namespace clang::tidy::automotive::cpp23

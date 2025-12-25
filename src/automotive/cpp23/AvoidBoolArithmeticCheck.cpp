//===--- AvoidBoolArithmeticCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidBoolArithmeticCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidBoolArithmeticCheck::registerMatchers(MatchFinder *Finder) {
  // Match binary operators that are NOT allowed with bool operands
  // Prohibited: arithmetic (+, -, *, /, %), bitwise (&, |, ^, <<, >>),
  //             relational (<, >, <=, >=)
  // Allowed: assignment (=), logical (&&, ||), equality (==, !=)
  Finder->addMatcher(
      binaryOperator(
          anyOf(
              // Arithmetic operators
              hasOperatorName("+"), hasOperatorName("-"), hasOperatorName("*"),
              hasOperatorName("/"), hasOperatorName("%"),
              // Bitwise operators (but NOT unary &)
              hasOperatorName("&"), hasOperatorName("|"), hasOperatorName("^"),
              hasOperatorName("<<"), hasOperatorName(">>"),
              // Relational operators
              hasOperatorName("<"), hasOperatorName(">"), hasOperatorName("<="),
              hasOperatorName(">="),
              // Compound assignment arithmetic
              hasOperatorName("+="), hasOperatorName("-="),
              hasOperatorName("*="), hasOperatorName("/="),
              hasOperatorName("%="),
              // Compound assignment bitwise
              hasOperatorName("&="), hasOperatorName("|="),
              hasOperatorName("^="), hasOperatorName("<<="),
              hasOperatorName(">>=")))
          .bind("binop"),
      this);

  // Match unary operators that are NOT allowed with bool operands
  // Prohibited: increment (++), decrement (--), bitwise NOT (~), unary + and -
  // Allowed: logical NOT (!), address-of (&)
  Finder->addMatcher(
      unaryOperator(anyOf(hasOperatorName("++"), hasOperatorName("--"),
                          hasOperatorName("~"), hasOperatorName("+"),
                          hasOperatorName("-")))
          .bind("unaryop"),
      this);
}

void AvoidBoolArithmeticCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop");
  const auto *UnaryOp = Result.Nodes.getNodeAs<UnaryOperator>("unaryop");

  if (BinOp) {
    checkBinaryOperator(BinOp, Result.Context);
  } else if (UnaryOp) {
    checkUnaryOperator(UnaryOp, Result.Context);
  }
}

bool AvoidBoolArithmeticCheck::isBoolType(QualType Type) const {
  // Remove references and get the canonical type
  Type = Type.getNonReferenceType().getCanonicalType();

  // Ignore const/volatile qualifiers
  Type = Type.getUnqualifiedType();

  // Check if it's bool type
  return Type->isBooleanType();
}

void AvoidBoolArithmeticCheck::checkBinaryOperator(const BinaryOperator *BinOp,
                                                   ASTContext *Context) {
  // Skip if in system header
  if (Context->getSourceManager().isInSystemHeader(BinOp->getOperatorLoc()))
    return;

  const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
  const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

  QualType LHSType = LHS->getType();
  QualType RHSType = RHS->getType();

  bool LHSIsBool = isBoolType(LHSType);
  bool RHSIsBool = isBoolType(RHSType);

  if (LHSIsBool || RHSIsBool) {
    StringRef OpStr = BinOp->getOpcodeStr();

    if (LHSIsBool && RHSIsBool) {
      diag(BinOp->getOperatorLoc(),
           "expression with type bool used with operator '%0'; "
           "only =, &&, ||, !, ==, != and unary & are allowed with bool")
          << OpStr;
    } else if (LHSIsBool) {
      diag(BinOp->getOperatorLoc(),
           "expression with type bool used as left operand of '%0'; "
           "only =, &&, ||, !, ==, != and unary & are allowed with bool")
          << OpStr;
    } else {
      diag(BinOp->getOperatorLoc(),
           "expression with type bool used as right operand of '%0'; "
           "only =, &&, ||, !, ==, != and unary & are allowed with bool")
          << OpStr;
    }
  }
}

void AvoidBoolArithmeticCheck::checkUnaryOperator(const UnaryOperator *UnaryOp,
                                                  ASTContext *Context) {
  // Skip if in system header
  if (Context->getSourceManager().isInSystemHeader(UnaryOp->getOperatorLoc()))
    return;

  const Expr *SubExpr = UnaryOp->getSubExpr()->IgnoreParenImpCasts();
  QualType SubExprType = SubExpr->getType();

  if (isBoolType(SubExprType)) {
    StringRef OpStr;
    switch (UnaryOp->getOpcode()) {
    case UO_PreInc:
    case UO_PostInc:
      OpStr = "++";
      break;
    case UO_PreDec:
    case UO_PostDec:
      OpStr = "--";
      break;
    case UO_Not:
      OpStr = "~";
      break;
    case UO_Plus:
      OpStr = "+";
      break;
    case UO_Minus:
      OpStr = "-";
      break;
    default:
      return; // Should not happen given our matcher
    }

    diag(UnaryOp->getOperatorLoc(),
         "expression with type bool used with operator '%0'; "
         "only =, &&, ||, !, ==, != and unary & are allowed with bool")
        << OpStr;
  }
}

} // namespace clang::tidy::automotive

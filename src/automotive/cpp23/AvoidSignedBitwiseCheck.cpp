//===--- AvoidSignedBitwiseCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSignedBitwiseCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidSignedBitwiseCheck::registerMatchers(MatchFinder *Finder) {
  // Match binary bitwise operators: &, |, ^, <<, >>
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("&"), hasOperatorName("|"),
                           hasOperatorName("^"), hasOperatorName("<<"),
                           hasOperatorName(">>")))
          .bind("binop"),
      this);

  // Match compound assignment bitwise operators: &=, |=, ^=, <<=, >>=
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("&="), hasOperatorName("|="),
                           hasOperatorName("^="), hasOperatorName("<<="),
                           hasOperatorName(">>=")))
          .bind("binop"),
      this);

  // Match unary bitwise NOT operator: ~
  Finder->addMatcher(unaryOperator(hasOperatorName("~")).bind("unaryop"), this);
}

void AvoidSignedBitwiseCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop");
  const auto *UnaryOp = Result.Nodes.getNodeAs<UnaryOperator>("unaryop");

  if (BinOp) {
    checkBinaryOperator(BinOp, Result.Context);
  } else if (UnaryOp) {
    checkUnaryOperator(UnaryOp, Result.Context);
  }
}

bool AvoidSignedBitwiseCheck::isSignedIntegerType(QualType Type) const {
  // Remove references and get the canonical type
  Type = Type.getNonReferenceType().getCanonicalType();

  // Ignore const/volatile qualifiers
  Type = Type.getUnqualifiedType();

  // Check if it's a signed integer type (but not an enum)
  if (Type->isSignedIntegerType() && !Type->isEnumeralType()) {
    return true;
  }

  return false;
}

void AvoidSignedBitwiseCheck::checkBinaryOperator(const BinaryOperator *BinOp,
                                                  ASTContext *Context) {
  // Skip if in system header
  if (Context->getSourceManager().isInSystemHeader(BinOp->getOperatorLoc()))
    return;

  const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
  const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

  QualType LHSType = LHS->getType();
  QualType RHSType = RHS->getType();

  bool LHSIsSigned = isSignedIntegerType(LHSType);
  bool RHSIsSigned = isSignedIntegerType(RHSType);

  if (LHSIsSigned || RHSIsSigned) {
    StringRef OpStr = BinOp->getOpcodeStr();

    if (LHSIsSigned && RHSIsSigned) {
      diag(BinOp->getOperatorLoc(),
           "bitwise operation '%0' performed on signed operands; "
           "use unsigned types instead")
          << OpStr;
    } else if (LHSIsSigned) {
      diag(BinOp->getOperatorLoc(),
           "bitwise operation '%0' performed with signed left operand; "
           "use unsigned types instead")
          << OpStr;
    } else {
      diag(BinOp->getOperatorLoc(),
           "bitwise operation '%0' performed with signed right operand; "
           "use unsigned types instead")
          << OpStr;
    }
  }
}

void AvoidSignedBitwiseCheck::checkUnaryOperator(const UnaryOperator *UnaryOp,
                                                 ASTContext *Context) {
  // Skip if in system header
  if (Context->getSourceManager().isInSystemHeader(UnaryOp->getOperatorLoc()))
    return;

  const Expr *SubExpr = UnaryOp->getSubExpr()->IgnoreParenImpCasts();
  QualType SubExprType = SubExpr->getType();

  if (isSignedIntegerType(SubExprType)) {
    diag(UnaryOp->getOperatorLoc(),
         "bitwise NOT operation '~' performed on signed operand; "
         "use unsigned types instead");
  }
}

} // namespace clang::tidy::automotive

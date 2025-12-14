//===--- AvoidCharacterArithmeticCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCharacterArithmeticCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidCharacterArithmeticCheck::registerMatchers(MatchFinder *Finder) {
  // Match addition and subtraction operators
  Finder->addMatcher(
      binaryOperator(hasAnyOperatorName("+", "-", "+=", "-=")).bind("binop"),
      this);
}

void AvoidCharacterArithmeticCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop");
  if (!BinOp)
    return;

  if (Result.SourceManager->isInSystemHeader(BinOp->getOperatorLoc()))
    return;

  const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
  const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

  bool LHSIsChar = isCharacterType(LHS->getType());
  bool RHSIsChar = isCharacterType(RHS->getType());

  // Rule 10.2: Both operands being character type is inappropriate
  if (LHSIsChar && RHSIsChar) {
    diag(BinOp->getOperatorLoc(),
         "addition or subtraction between two expressions of essentially "
         "character type is not allowed");
  }
}

bool AvoidCharacterArithmeticCheck::isCharacterType(QualType Type) const {
  Type = Type.getCanonicalType().getUnqualifiedType();
  return Type->isAnyCharacterType();
}

} // namespace clang::tidy::automotive

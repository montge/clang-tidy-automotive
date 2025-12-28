//===--- WrongConditionalOperandTypeCheck.cpp - clang-tidy ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "WrongConditionalOperandTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void WrongConditionalOperandTypeCheck::registerMatchers(MatchFinder *Finder) {
  // Match conditional operator (ternary operator)
  Finder->addMatcher(conditionalOperator().bind("cond"), this);
}

void WrongConditionalOperandTypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *CondOp = Result.Nodes.getNodeAs<ConditionalOperator>("cond");
  if (!CondOp)
    return;

  if (Result.SourceManager->isInSystemHeader(CondOp->getQuestionLoc()))
    return;

  const Expr *TrueExpr = CondOp->getTrueExpr()->IgnoreParenImpCasts();
  const Expr *FalseExpr = CondOp->getFalseExpr()->IgnoreParenImpCasts();

  QualType TrueType = TrueExpr->getType();
  QualType FalseType = FalseExpr->getType();

  if (TrueType.isNull() || FalseType.isNull())
    return;

  bool TrueIsPointer = TrueType->isPointerType();
  bool FalseIsPointer = FalseType->isPointerType();
  bool TrueIsInteger = TrueType->isIntegerType();
  bool FalseIsInteger = FalseType->isIntegerType();

  // Check if one operand is pointer and the other is integer
  if ((TrueIsPointer && FalseIsInteger) || (TrueIsInteger && FalseIsPointer)) {
    // Allow NULL pointer constant (integer constant expression with value 0)
    const Expr *IntExpr = FalseIsInteger ? FalseExpr : TrueExpr;

    // Check if it's a null pointer constant
    if (IntExpr->isNullPointerConstant(*Result.Context,
                                       Expr::NPC_ValueDependentIsNotNull) !=
        Expr::NPCK_NotNull) {
      // This is a null pointer constant, which is allowed
      return;
    }

    diag(CondOp->getQuestionLoc(),
         "incompatible pointer and integer types used as operands to "
         "conditional operator");
  }
}

} // namespace clang::tidy::automotive

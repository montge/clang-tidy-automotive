//===--- InvariantControlCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "InvariantControlCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void InvariantControlCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      ifStmt(hasCondition(expr().bind("controlExpr"))).bind("ifStmt"), this);

  Finder->addMatcher(
      whileStmt(hasCondition(expr().bind("controlExpr"))).bind("whileStmt"),
      this);

  Finder->addMatcher(
      doStmt(hasCondition(expr().bind("controlExpr"))).bind("doStmt"), this);

  Finder->addMatcher(
      forStmt(hasCondition(expr().bind("controlExpr"))).bind("forStmt"), this);

  Finder->addMatcher(
      switchStmt(hasCondition(expr().bind("controlExpr"))).bind("switchStmt"),
      this);

  Finder->addMatcher(
      conditionalOperator(hasCondition(expr().bind("controlExpr")))
          .bind("ternaryOp"),
      this);
}

void InvariantControlCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ControlExpr = Result.Nodes.getNodeAs<Expr>("controlExpr");
  Expr::EvalResult EvalResult;

  if (!ControlExpr)
    return;

  // Don't evaluate value-dependent expressions (e.g., in templates)
  if (ControlExpr->isValueDependent())
    return;

  if (ControlExpr->EvaluateAsInt(EvalResult, *Result.Context)) {
    if (EvalResult.Val.getInt().getBoolValue()) {
      diag(ControlExpr->getBeginLoc(),
           "avoid controlling expression to always be evaluated to 'true'");
    } else {
      diag(ControlExpr->getBeginLoc(),
           "avoid controlling expression to always be evaluated to 'false'");
    }
  }
}

} // namespace clang::tidy::automotive

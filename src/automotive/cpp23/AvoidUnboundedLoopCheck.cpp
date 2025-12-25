//===--- AvoidUnboundedLoopCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnboundedLoopCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidUnboundedLoopCheck::registerMatchers(MatchFinder *Finder) {
  // Match for loops with no condition (for(;;))
  Finder->addMatcher(
      forStmt(unless(hasCondition(expr()))).bind("forNoCondition"), this);

  // Match while loops with constant true conditions
  Finder->addMatcher(
      whileStmt(hasCondition(expr().bind("whileCondition"))).bind("whileStmt"),
      this);

  // Match do-while loops with constant true conditions
  Finder->addMatcher(
      doStmt(hasCondition(expr().bind("doCondition"))).bind("doStmt"), this);

  // Match for loops with constant true conditions
  Finder->addMatcher(
      forStmt(hasCondition(expr().bind("forCondition"))).bind("forStmt"), this);
}

bool AvoidUnboundedLoopCheck::isConstantCondition(const Expr *E,
                                                  ASTContext &Context) const {
  if (!E)
    return false;

  // Skip value-dependent expressions (e.g., in templates)
  if (E->isValueDependent())
    return false;

  // Check if it evaluates to a constant
  Expr::EvalResult EvalResult;
  if (E->EvaluateAsInt(EvalResult, Context)) {
    return true;
  }

  // Additional check for boolean literal expressions
  if (const auto *BoolLiteral =
          dyn_cast<CXXBoolLiteralExpr>(E->IgnoreParenCasts())) {
    return true;
  }

  // Check for integer literals used as conditions
  if (const auto *IntLiteral =
          dyn_cast<IntegerLiteral>(E->IgnoreParenCasts())) {
    return true;
  }

  return false;
}

bool AvoidUnboundedLoopCheck::isTrueConstant(const Expr *E,
                                             ASTContext &Context) const {
  if (!E)
    return false;

  // Skip value-dependent expressions
  if (E->isValueDependent())
    return false;

  // Check if it evaluates to true
  Expr::EvalResult EvalResult;
  if (E->EvaluateAsInt(EvalResult, Context)) {
    return EvalResult.Val.getInt().getBoolValue();
  }

  // Check for true literal
  if (const auto *BoolLiteral =
          dyn_cast<CXXBoolLiteralExpr>(E->IgnoreParenCasts())) {
    return BoolLiteral->getValue();
  }

  // Check for non-zero integer literals
  if (const auto *IntLiteral =
          dyn_cast<IntegerLiteral>(E->IgnoreParenCasts())) {
    return IntLiteral->getValue().getBoolValue();
  }

  return false;
}

void AvoidUnboundedLoopCheck::check(const MatchFinder::MatchResult &Result) {

  // Check for loops with no condition (for(;;))
  if (const auto *ForNoCondition =
          Result.Nodes.getNodeAs<ForStmt>("forNoCondition")) {
    if (Result.SourceManager->isInSystemHeader(ForNoCondition->getForLoc()))
      return;

    diag(ForNoCondition->getForLoc(), "loop has no condition; ensure loop has "
                                      "well-defined termination bounds")
        << ForNoCondition->getSourceRange();
    return;
  }

  // Check while loops
  if (const auto *WhileLoop = Result.Nodes.getNodeAs<WhileStmt>("whileStmt")) {
    const auto *WhileCondition = Result.Nodes.getNodeAs<Expr>("whileCondition");
    if (WhileCondition &&
        !Result.SourceManager->isInSystemHeader(WhileLoop->getWhileLoc())) {
      if (isTrueConstant(WhileCondition, *Result.Context)) {
        diag(WhileCondition->getBeginLoc(),
             "while loop has constant true condition; ensure loop has "
             "well-defined "
             "termination bounds")
            << WhileCondition->getSourceRange();
      }
    }
    return;
  }

  // Check do-while loops
  if (const auto *DoLoop = Result.Nodes.getNodeAs<DoStmt>("doStmt")) {
    const auto *DoCondition = Result.Nodes.getNodeAs<Expr>("doCondition");
    if (DoCondition &&
        !Result.SourceManager->isInSystemHeader(DoLoop->getDoLoc())) {
      if (isTrueConstant(DoCondition, *Result.Context)) {
        diag(DoCondition->getBeginLoc(),
             "do-while loop has constant true condition; ensure loop has "
             "well-defined termination bounds")
            << DoCondition->getSourceRange();
      }
    }
    return;
  }

  // Check for loops with constant conditions
  if (const auto *ForLoop = Result.Nodes.getNodeAs<ForStmt>("forStmt")) {
    const auto *ForCondition = Result.Nodes.getNodeAs<Expr>("forCondition");
    if (ForCondition &&
        !Result.SourceManager->isInSystemHeader(ForLoop->getForLoc())) {
      if (isTrueConstant(ForCondition, *Result.Context)) {
        diag(ForCondition->getBeginLoc(),
             "for loop has constant true condition; ensure loop has "
             "well-defined "
             "termination bounds")
            << ForCondition->getSourceRange();
      }
    }
    return;
  }
}

} // namespace clang::tidy::automotive::cpp23

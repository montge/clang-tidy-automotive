//===--- MultipleReturnStmtCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MultipleReturnStmtCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MultipleReturnStmtCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(forEachDescendant(returnStmt().bind("returnStmt")))
          .bind("func"),
      this);
}

void MultipleReturnStmtCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedFunc = Result.Nodes.getNodeAs<FunctionDecl>("func");
  const auto *MatchedReturn = Result.Nodes.getNodeAs<ReturnStmt>("returnStmt");

  if (!MatchedFunc || !MatchedReturn)
    return;

  if (MatchedFunc != CurrentFunc) {
    CurrentFunc = MatchedFunc;
    PreviousReturn = MatchedReturn;
    return;
  }

  if (PreviousReturn) {
    diag(PreviousReturn->getBeginLoc(), "avoid multiple return statement");
    diag(MatchedFunc->getBeginLoc(),
         "multiple return statement within function", DiagnosticIDs::Note);
    PreviousReturn = nullptr;
  }
  diag(MatchedReturn->getBeginLoc(), "avoid multiple return statement");
}

} // namespace clang::tidy::automotive

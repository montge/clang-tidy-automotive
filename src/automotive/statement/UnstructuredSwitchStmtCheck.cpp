//===--- UnstructuredSwitchStmtCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnstructuredSwitchStmtCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

StatementMatcher matchUnstructuredSwitch() {
  return allOf(unless(hasParent(compoundStmt(hasParent(switchStmt())))),
               hasParent(stmt(hasParent(stmt().bind("expectedSwitch")))
                             .bind("expectedCompound")));
}

void UnstructuredSwitchStmtCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(caseStmt(matchUnstructuredSwitch()).bind("case"), this);
  Finder->addMatcher(defaultStmt(matchUnstructuredSwitch()).bind("default"),
                     this);
}

void UnstructuredSwitchStmtCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedCase = Result.Nodes.getNodeAs<CaseStmt>("case");
  const auto *MatchedDefault = Result.Nodes.getNodeAs<DefaultStmt>("default");
  const auto *ExpectedCompound =
      Result.Nodes.getNodeAs<Stmt>("expectedCompound");
  const auto *ExpectedSwitch = Result.Nodes.getNodeAs<Stmt>("expectedSwitch");

  if (MatchedCase)
    diag(MatchedCase->getCaseLoc(), "case statement at the wrong place");
  else if (MatchedDefault)
    diag(MatchedDefault->getDefaultLoc(),
         "default statement at the wrong place");

  if (ExpectedSwitch) {
    diag(ExpectedSwitch->getBeginLoc(), "expected switch statement",
         DiagnosticIDs::Note);
  }
  if (ExpectedCompound) {
    diag(ExpectedCompound->getBeginLoc(), "expected compound statement",
         DiagnosticIDs::Note);
  }
}

} // namespace clang::tidy::automotive

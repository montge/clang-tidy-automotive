//===--- AvoidSingleClauseSwitchCppCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSingleClauseSwitchCppCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidSingleClauseSwitchCppCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(switchStmt().bind("switch"), this);
}

void AvoidSingleClauseSwitchCppCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Switch = Result.Nodes.getNodeAs<SwitchStmt>("switch");
  if (!Switch)
    return;

  if (Result.SourceManager->isInSystemHeader(Switch->getSwitchLoc()))
    return;

  // Count the number of switch cases (case and default)
  unsigned ClauseCount = 0;
  for (const SwitchCase *SC = Switch->getSwitchCaseList(); SC;
       SC = SC->getNextSwitchCase()) {
    ++ClauseCount;
  }

  if (ClauseCount < 2) {
    diag(Switch->getSwitchLoc(),
         "switch statement shall have at least two non-empty switch-clauses");
  }
}

} // namespace clang::tidy::automotive

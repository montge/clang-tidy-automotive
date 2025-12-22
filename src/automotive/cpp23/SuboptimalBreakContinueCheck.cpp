//===--- SuboptimalBreakContinueCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SuboptimalBreakContinueCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void SuboptimalBreakContinueCheck::registerMatchers(MatchFinder *Finder) {
  // Match break statements inside compound statements
  Finder->addMatcher(
      compoundStmt(forEach(breakStmt().bind("break"))).bind("compound"), this);

  // Match continue statements inside compound statements
  Finder->addMatcher(
      compoundStmt(forEach(continueStmt().bind("continue"))).bind("compound"),
      this);
}

void SuboptimalBreakContinueCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Compound = Result.Nodes.getNodeAs<CompoundStmt>("compound");
  const auto *Break = Result.Nodes.getNodeAs<BreakStmt>("break");
  const auto *Continue = Result.Nodes.getNodeAs<ContinueStmt>("continue");

  if (!Compound)
    return;

  const Stmt *TargetStmt = Break ? static_cast<const Stmt *>(Break)
                                 : static_cast<const Stmt *>(Continue);
  if (!TargetStmt)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(TargetStmt->getBeginLoc()))
    return;

  // Find the position of the break/continue statement in the compound statement
  auto CompoundChildren = Compound->body();
  auto It =
      std::find(CompoundChildren.begin(), CompoundChildren.end(), TargetStmt);

  // If not found directly, the break/continue might be nested
  // (e.g., inside an if statement without braces)
  if (It == CompoundChildren.end())
    return;

  // Check if there are any statements after this one
  ++It;
  if (It != CompoundChildren.end()) {
    // There are statements after the break/continue
    const char *StmtType = Break ? "break" : "continue";
    diag(TargetStmt->getBeginLoc(),
         "%0 statement should be the last statement in the compound statement")
        << StmtType;
  }
}

} // namespace clang::tidy::automotive::cpp23

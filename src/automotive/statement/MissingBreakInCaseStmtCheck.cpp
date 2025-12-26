//===--- MissingBreakInCaseStmtCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingBreakInCaseStmtCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
/// Check if a statement contains an unconditional terminator (break, return,
/// goto, continue). This recursively checks the last statement in compound
/// statements.
bool containsUnconditionalTerminator(const Stmt *S) {
  if (!S)
    return false;

  // Direct terminators
  if (isa<BreakStmt>(S) || isa<ReturnStmt>(S) || isa<GotoStmt>(S) ||
      isa<ContinueStmt>(S) || isa<CXXThrowExpr>(S))
    return true;

  // For compound statements, check the last statement
  if (const auto *Compound = dyn_cast<CompoundStmt>(S)) {
    if (Compound->body_empty())
      return false;
    return containsUnconditionalTerminator(Compound->body_back());
  }

  // For case statements, check the sub-statement
  if (const auto *Case = dyn_cast<CaseStmt>(S))
    return containsUnconditionalTerminator(Case->getSubStmt());

  // For default statements, check the sub-statement
  if (const auto *Default = dyn_cast<DefaultStmt>(S))
    return containsUnconditionalTerminator(Default->getSubStmt());

  return false;
}

/// Get all statements within a case (including sub-statements and siblings
/// until the next case).
bool caseEndsWithTerminator(const Stmt *Case,
                            CompoundStmt::const_body_iterator It,
                            const CompoundStmt *Body) {
  // First, check the sub-statement of the case itself
  const Stmt *SubStmt = nullptr;
  if (const auto *C = dyn_cast<CaseStmt>(Case))
    SubStmt = C->getSubStmt();
  else if (const auto *D = dyn_cast<DefaultStmt>(Case))
    SubStmt = D->getSubStmt();

  if (SubStmt && containsUnconditionalTerminator(SubStmt))
    return true;

  // Then check sibling statements until the next case
  for (++It; It != Body->body_end(); ++It) {
    const Stmt *S = *It;

    // If we hit another case, stop
    if (isa<CaseStmt>(S) || isa<DefaultStmt>(S))
      return false;

    // Check if this statement is a terminator
    if (containsUnconditionalTerminator(S))
      return true;
  }

  return false;
}
} // namespace

void MissingBreakInCaseStmtCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(switchStmt().bind("switchStmt"), this);
}

void MissingBreakInCaseStmtCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Switch = Result.Nodes.getNodeAs<SwitchStmt>("switchStmt");
  if (!Switch)
    return;

  const auto *Body = dyn_cast<CompoundStmt>(Switch->getBody());
  if (!Body)
    return;

  for (auto It = Body->body_begin(); It != Body->body_end(); ++It) {
    const Stmt *S = *It;

    if (const auto *Case = dyn_cast<CaseStmt>(S)) {
      checkCase(Case, It, Body, Result.Context);
    } else if (const auto *Default = dyn_cast<DefaultStmt>(S)) {
      checkCase(Default, It, Body, Result.Context);
    }
  }
}

void MissingBreakInCaseStmtCheck::checkCase(
    const Stmt *Case, CompoundStmt::const_body_iterator It,
    const CompoundStmt *Body, ASTContext *Context) {
  // Check for empty fall-through (e.g., case 1: case 2: break;)
  if (const auto *C = dyn_cast<CaseStmt>(Case)) {
    const Stmt *Sub = C->getSubStmt();
    // If the sub-statement is another case or default, this is an empty
    // fall-through which is allowed
    if (Sub && (isa<CaseStmt>(Sub) || isa<DefaultStmt>(Sub)))
      return;
  }

  // Check if case ends with an unconditional terminator
  if (!caseEndsWithTerminator(Case, It, Body)) {
    diag(Case->getBeginLoc(),
         "every non-empty switch case must end with a break statement");
  }
}

} // namespace clang::tidy::automotive

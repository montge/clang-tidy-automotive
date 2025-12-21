//===--- AvoidNestedSwitchLabelCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNestedSwitchLabelCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidNestedSwitchLabelCheck::registerMatchers(MatchFinder *Finder) {
  // Match case statements that have a compound statement ancestor
  // between them and the switch statement
  Finder->addMatcher(
      caseStmt(hasAncestor(switchStmt().bind("switch"))).bind("case"), this);
  Finder->addMatcher(
      defaultStmt(hasAncestor(switchStmt().bind("switch"))).bind("default"),
      this);
}

void AvoidNestedSwitchLabelCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Switch = Result.Nodes.getNodeAs<SwitchStmt>("switch");
  const Stmt *Label = Result.Nodes.getNodeAs<CaseStmt>("case");
  if (!Label)
    Label = Result.Nodes.getNodeAs<DefaultStmt>("default");

  if (!Switch || !Label)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Label->getBeginLoc()))
    return;

  // Get the switch body
  const Stmt *SwitchBody = Switch->getBody();
  if (!SwitchBody)
    return;

  // Walk up the parent chain from the label to the switch body
  // If we encounter any compound statement that is not the switch body itself,
  // then the label is nested
  ASTContext &Ctx = *Result.Context;
  const Stmt *Current = Label;

  while (Current) {
    // Get the parent
    auto Parents = Ctx.getParents(*Current);
    if (Parents.empty())
      break;

    const Stmt *Parent = Parents[0].get<Stmt>();
    if (!Parent)
      break;

    // If we reached the switch body directly, label is at outermost level
    if (Parent == SwitchBody)
      return;

    // If we reached the switch itself, label is at outermost level
    if (Parent == Switch)
      return;

    // If we hit a compound statement that's not the switch body,
    // the label is nested
    if (isa<CompoundStmt>(Parent) && Parent != SwitchBody) {
      const char *LabelType =
          isa<CaseStmt>(Label) ? "case label" : "default label";
      diag(Label->getBeginLoc(),
           "%0 is nested inside a compound statement; switch labels should "
           "only appear in the outermost compound statement of the switch body")
          << LabelType;
      return;
    }

    Current = Parent;
  }
}

} // namespace clang::tidy::automotive

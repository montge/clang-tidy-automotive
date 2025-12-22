//===--- WellFormedSwitchCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "WellFormedSwitchCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void WellFormedSwitchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(switchStmt().bind("switch"), this);
}

bool WellFormedSwitchCheck::hasTerminatingStatement(
    const Stmt *S, const ASTContext &Context) const {
  if (!S)
    return false;

  // Check for explicit terminating statements
  if (isa<BreakStmt>(S) || isa<ReturnStmt>(S) || isa<ContinueStmt>(S) ||
      isa<CXXThrowExpr>(S) || isa<GotoStmt>(S))
    return true;

  // For compound statements, check the last statement
  if (const auto *CS = dyn_cast<CompoundStmt>(S)) {
    if (CS->body_empty())
      return false;
    return hasTerminatingStatement(CS->body_back(), Context);
  }

  // For if statements, both branches must terminate
  if (const auto *If = dyn_cast<IfStmt>(S)) {
    const Stmt *Then = If->getThen();
    const Stmt *Else = If->getElse();
    if (!Else)
      return false;
    return hasTerminatingStatement(Then, Context) &&
           hasTerminatingStatement(Else, Context);
  }

  // Call to noreturn function
  if (const auto *CE = dyn_cast<CallExpr>(S)) {
    if (const auto *FD = CE->getDirectCallee()) {
      if (FD->isNoReturn())
        return true;
    }
  }

  return false;
}

void WellFormedSwitchCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Switch = Result.Nodes.getNodeAs<SwitchStmt>("switch");
  if (!Switch)
    return;

  const Stmt *Body = Switch->getBody();
  if (!Body)
    return;

  bool HasDefault = false;
  unsigned NumCases = 0;

  // Collect all cases and check for default
  for (const SwitchCase *SC = Switch->getSwitchCaseList(); SC;
       SC = SC->getNextSwitchCase()) {
    if (isa<DefaultStmt>(SC))
      HasDefault = true;
    else if (isa<CaseStmt>(SC))
      NumCases++;
  }

  // Check for missing default (unless all enum values are covered)
  if (!HasDefault) {
    QualType CondType = Switch->getCond()->IgnoreParenImpCasts()->getType();
    bool IsFullEnumCoverage = false;

    // Look through typedefs to find the underlying enum type
    if (const auto *TDT = CondType->getAs<TypedefType>()) {
      CondType = TDT->desugar();
    }
    CondType = CondType.getCanonicalType();

    if (const auto *ET = CondType->getAs<EnumType>()) {
      const EnumDecl *ED = ET->getDecl()->getDefinition();
      if (ED) {
        unsigned NumEnumerators = 0;
        for (const auto *E : ED->enumerators())
          (void)E, NumEnumerators++;
        IsFullEnumCoverage = (NumCases == NumEnumerators);
      }
    }

    if (!IsFullEnumCoverage) {
      diag(Switch->getSwitchLoc(),
           "switch statement does not have a 'default' clause");
    }
  }

  // Check for fall-through: iterate through switch cases
  // We need to find cases that don't end with a terminating statement
  // and are followed by another case

  // Get the compound statement body if it exists
  const CompoundStmt *CS = dyn_cast<CompoundStmt>(Body);
  if (!CS)
    return;

  // Iterate through the body to find case labels and check for termination
  SmallVector<std::pair<const SwitchCase *, const Stmt *>, 16> CaseStmts;

  const SwitchCase *CurrentCase = nullptr;
  const Stmt *LastNonCaseStmt = nullptr;

  for (const Stmt *S : CS->body()) {
    if (const auto *SC = dyn_cast<SwitchCase>(S)) {
      // If there was a previous case, check if it terminated properly
      if (CurrentCase && LastNonCaseStmt) {
        if (!hasTerminatingStatement(LastNonCaseStmt, *Result.Context)) {
          diag(CurrentCase->getKeywordLoc(),
               "case does not end with 'break', 'return', 'continue', or "
               "'throw'; implicit fall-through is not allowed");
        }
      }

      // Walk the nested case chain to find the actual sub-statement
      const SwitchCase *Nested = SC;
      while (const auto *NextCase =
                 dyn_cast_or_null<SwitchCase>(Nested->getSubStmt())) {
        Nested = NextCase;
      }
      CurrentCase = SC;
      LastNonCaseStmt = Nested->getSubStmt();

      // If sub-statement is null or another case, this is an empty case (ok for
      // fall-through)
      if (!LastNonCaseStmt || isa<SwitchCase>(LastNonCaseStmt)) {
        CurrentCase =
            nullptr; // Mark as empty case, don't check for termination
        LastNonCaseStmt = nullptr;
      }
    } else {
      // Regular statement - update last non-case statement
      LastNonCaseStmt = S;
    }
  }

  // Don't need to check the last case - it's OK if it falls through to the end
}

} // namespace clang::tidy::automotive

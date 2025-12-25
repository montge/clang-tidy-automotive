//===--- CondVarMutexAssociationCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CondVarMutexAssociationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void CondVarMutexAssociationCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to cnd_wait, cnd_timedwait, pthread_cond_wait,
  // pthread_cond_timedwait
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName(
                   "cnd_wait", "cnd_timedwait", "pthread_cond_wait",
                   "pthread_cond_timedwait"))))
          .bind("condWait"),
      this);
}

static std::string getArgName(const Expr *E) {
  E = E->IgnoreParenImpCasts();

  // Handle &var
  if (const auto *UO = dyn_cast<UnaryOperator>(E)) {
    if (UO->getOpcode() == UO_AddrOf) {
      E = UO->getSubExpr()->IgnoreParenImpCasts();
    }
  }

  // Get declaration reference
  if (const auto *DRE = dyn_cast<DeclRefExpr>(E)) {
    return DRE->getDecl()->getNameAsString();
  }

  // Handle member expressions
  if (const auto *ME = dyn_cast<MemberExpr>(E)) {
    return ME->getMemberDecl()->getNameAsString();
  }

  return "";
}

void CondVarMutexAssociationCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("condWait");
  if (!Call || Call->getNumArgs() < 2)
    return;

  SM = Result.SourceManager;

  // Skip system headers
  if (SM->isInSystemHeader(Call->getBeginLoc()))
    return;

  // Get condition variable name (first argument)
  std::string CondVarName = getArgName(Call->getArg(0));
  if (CondVarName.empty())
    return;

  // Get mutex name (second argument)
  std::string MutexName = getArgName(Call->getArg(1));
  if (MutexName.empty())
    return;

  // Record this association
  MutexUsage Usage;
  Usage.MutexName = MutexName;
  Usage.Loc = Call->getBeginLoc();
  CondVarMutexMap[CondVarName].push_back(Usage);
}

void CondVarMutexAssociationCheck::onEndOfTranslationUnit() {
  // Check each condition variable for multiple mutex associations
  for (const auto &Entry : CondVarMutexMap) {
    const std::string &CondVarName = Entry.first;
    const std::vector<MutexUsage> &Usages = Entry.second;

    if (Usages.size() < 2)
      continue;

    // Collect unique mutex names
    std::set<std::string> UniqueMutexes;
    for (const auto &Usage : Usages) {
      UniqueMutexes.insert(Usage.MutexName);
    }

    // If more than one unique mutex, report
    if (UniqueMutexes.size() > 1) {
      // Report at the second occurrence with a different mutex
      std::string FirstMutex = Usages[0].MutexName;
      for (size_t i = 1; i < Usages.size(); ++i) {
        if (Usages[i].MutexName != FirstMutex) {
          diag(Usages[i].Loc,
               "condition variable '%0' is used with multiple mutexes; "
               "previously used with '%1', now with '%2'")
              << CondVarName << FirstMutex << Usages[i].MutexName;
          diag(Usages[0].Loc, "first association with mutex '%0' was here",
               DiagnosticIDs::Note)
              << FirstMutex;
          break; // Only report once per condition variable
        }
      }
    }
  }

  // Clear for next translation unit
  CondVarMutexMap.clear();
}

} // namespace clang::tidy::automotive

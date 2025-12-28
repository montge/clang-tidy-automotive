//===--- SpuriousWakeupCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SpuriousWakeupCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void SpuriousWakeupCheck::registerMatchers(MatchFinder *Finder) {
  // Match condition variable wait calls that are NOT in a loop
  Finder->addMatcher(
      callExpr(
          callee(functionDecl(hasAnyName(
              "cnd_wait", "cnd_timedwait",
              "pthread_cond_wait", "pthread_cond_timedwait"))),
          // Not enclosed in a loop statement
          unless(hasAncestor(stmt(anyOf(
              whileStmt(),
              forStmt(),
              doStmt()
          ))))
      ).bind("condWait"),
      this);

  // Also match waits in loops but without an obvious predicate check
  // This is a more sophisticated check
  Finder->addMatcher(
      callExpr(
          callee(functionDecl(hasAnyName(
              "cnd_wait", "cnd_timedwait",
              "pthread_cond_wait", "pthread_cond_timedwait"))),
          hasAncestor(whileStmt(
              // Check if the condition is just 'true' or '1' (infinite loop without predicate)
              hasCondition(anyOf(
                  cxxBoolLiteral(equals(true)),
                  integerLiteral(equals(1))
              ))
          ).bind("whileTrue"))
      ).bind("condWaitInWhileTrue"),
      this);
}

void SpuriousWakeupCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Handle wait not in a loop
  if (const auto *WaitCall = Result.Nodes.getNodeAs<CallExpr>("condWait")) {
    if (Result.SourceManager->isInSystemHeader(WaitCall->getBeginLoc()))
      return;

    const FunctionDecl *Callee = WaitCall->getDirectCallee();
    if (!Callee)
      return;

    diag(WaitCall->getBeginLoc(),
         "condition variable wait using '%0' is not enclosed in a loop; "
         "spurious wakeups must be handled by rechecking the predicate condition "
         "(MISRA C:2025 Rule 22.20)")
        << Callee->getName();
  }

  // Handle wait in while(true) or while(1) without proper predicate
  if (const auto *WaitCall = Result.Nodes.getNodeAs<CallExpr>("condWaitInWhileTrue")) {
    if (Result.SourceManager->isInSystemHeader(WaitCall->getBeginLoc()))
      return;

    const auto *WhileTrue = Result.Nodes.getNodeAs<WhileStmt>("whileTrue");
    if (!WhileTrue)
      return;

    const FunctionDecl *Callee = WaitCall->getDirectCallee();
    if (!Callee)
      return;

    diag(WaitCall->getBeginLoc(),
         "condition variable wait using '%0' is in a loop with constant true condition; "
         "loop condition should check a meaningful predicate to handle spurious wakeups "
         "(MISRA C:2025 Rule 22.20)")
        << Callee->getName();
    diag(WhileTrue->getBeginLoc(),
         "loop with constant true condition starts here",
         DiagnosticIDs::Note);
  }
}

} // namespace clang::tidy::automotive

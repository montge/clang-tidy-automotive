//===--- AvoidNestedSyncCallsCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNestedSyncCallsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

bool AvoidNestedSyncCallsCheck::isSyncFunction(StringRef FuncName) const {
  // List of C11 thread synchronization functions
  return FuncName == "mtx_lock" || FuncName == "mtx_timedlock" ||
         FuncName == "mtx_trylock" || FuncName == "mtx_unlock" ||
         FuncName == "cnd_wait" || FuncName == "cnd_timedwait" ||
         FuncName == "cnd_signal" || FuncName == "cnd_broadcast" ||
         FuncName == "call_once" ||
         // POSIX thread functions (commonly used)
         FuncName == "pthread_mutex_lock" ||
         FuncName == "pthread_mutex_trylock" ||
         FuncName == "pthread_mutex_unlock" ||
         FuncName == "pthread_cond_wait" ||
         FuncName == "pthread_cond_timedwait" ||
         FuncName == "pthread_cond_signal" ||
         FuncName == "pthread_cond_broadcast";
}

void AvoidNestedSyncCallsCheck::registerMatchers(MatchFinder *Finder) {
  // Match synchronization function calls
  Finder->addMatcher(
      callExpr(
          callee(functionDecl(hasAnyName(
                                  "mtx_lock", "mtx_timedlock", "mtx_trylock",
                                  "mtx_unlock", "cnd_wait", "cnd_timedwait",
                                  "cnd_signal", "cnd_broadcast", "call_once",
                                  "pthread_mutex_lock", "pthread_mutex_trylock",
                                  "pthread_mutex_unlock", "pthread_cond_wait",
                                  "pthread_cond_timedwait",
                                  "pthread_cond_signal",
                                  "pthread_cond_broadcast"))
                     .bind("sync_func")),
          // Look for another sync call as an ancestor
          hasAncestor(callExpr(callee(functionDecl(hasAnyName(
                                           "mtx_lock", "mtx_timedlock",
                                           "mtx_trylock", "mtx_unlock",
                                           "cnd_wait", "cnd_timedwait",
                                           "cnd_signal", "cnd_broadcast",
                                           "call_once", "pthread_mutex_lock",
                                           "pthread_mutex_trylock",
                                           "pthread_mutex_unlock",
                                           "pthread_cond_wait",
                                           "pthread_cond_timedwait",
                                           "pthread_cond_signal",
                                           "pthread_cond_broadcast"))))
                          .bind("parent_sync")))
          .bind("nested_call"),
      this);
}

void AvoidNestedSyncCallsCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *NestedCall = Result.Nodes.getNodeAs<CallExpr>("nested_call");
  const auto *ParentSync = Result.Nodes.getNodeAs<CallExpr>("parent_sync");

  if (!NestedCall || !ParentSync)
    return;

  if (Result.SourceManager->isInSystemHeader(NestedCall->getBeginLoc()))
    return;

  // Avoid reporting the same call
  if (NestedCall == ParentSync)
    return;

  const auto *NestedCallee = NestedCall->getDirectCallee();
  const auto *ParentCallee = ParentSync->getDirectCallee();

  if (!NestedCallee || !ParentCallee)
    return;

  diag(NestedCall->getBeginLoc(),
       "nested call to thread synchronization function '%0' detected; calls "
       "shall not be nested")
      << NestedCallee->getName();

  diag(ParentSync->getBeginLoc(), "parent synchronization call is here",
       DiagnosticIDs::Note);
}

} // namespace clang::tidy::automotive

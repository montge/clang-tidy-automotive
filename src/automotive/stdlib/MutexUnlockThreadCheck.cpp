//===--- MutexUnlockThreadCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MutexUnlockThreadCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MutexUnlockThreadCheck::registerMatchers(MatchFinder *Finder) {
  // Match mutex lock calls
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName(
                   "mtx_lock", "pthread_mutex_lock",
                   "mtx_timedlock", "pthread_mutex_timedlock",
                   "mtx_trylock", "pthread_mutex_trylock"))))
          .bind("lock"),
      this);

  // Match mutex unlock calls
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName(
                   "mtx_unlock", "pthread_mutex_unlock"))))
          .bind("unlock"),
      this);
}

static std::string getMutexName(const Expr *E) {
  E = E->IgnoreParenImpCasts();

  // Handle &mutex
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

static std::string getFunctionName(const Stmt *S, ASTContext *Context) {
  // Traverse up to find the enclosing function
  const auto &Parents = Context->getParents(*S);
  for (const auto &Parent : Parents) {
    if (const auto *FD = Parent.get<FunctionDecl>()) {
      return FD->getNameAsString();
    }
    if (const auto *ParentStmt = Parent.get<Stmt>()) {
      return getFunctionName(ParentStmt, Context);
    }
  }
  return "";
}

void MutexUnlockThreadCheck::check(
    const MatchFinder::MatchResult &Result) {
  SM = Result.SourceManager;

  // Handle lock
  if (const auto *LockCall = Result.Nodes.getNodeAs<CallExpr>("lock")) {
    if (SM->isInSystemHeader(LockCall->getBeginLoc()))
      return;

    if (LockCall->getNumArgs() < 1)
      return;

    std::string MutexName = getMutexName(LockCall->getArg(0));
    if (MutexName.empty())
      return;

    std::string FuncName = getFunctionName(LockCall, Result.Context);

    // Record the lock with the function that performed it
    MutexLocks[MutexName] = {FuncName, LockCall->getBeginLoc()};
  }

  // Handle unlock
  if (const auto *UnlockCall = Result.Nodes.getNodeAs<CallExpr>("unlock")) {
    if (SM->isInSystemHeader(UnlockCall->getBeginLoc()))
      return;

    if (UnlockCall->getNumArgs() < 1)
      return;

    std::string MutexName = getMutexName(UnlockCall->getArg(0));
    if (MutexName.empty())
      return;

    std::string FuncName = getFunctionName(UnlockCall, Result.Context);

    // Check if this mutex was locked
    auto It = MutexLocks.find(MutexName);
    if (It != MutexLocks.end()) {
      // If locked and unlocked from different functions, warn
      // Note: This is a heuristic - different functions may be called from the same thread
      if (!It->second.FunctionName.empty() &&
          !FuncName.empty() &&
          It->second.FunctionName != FuncName) {
        diag(UnlockCall->getBeginLoc(),
             "mutex '%0' locked in function '%1' but unlocked in function '%2'; "
             "ensure lock and unlock occur in the same thread (MISRA C:2025 Rule 22.17)")
            << MutexName << It->second.FunctionName << FuncName;
        diag(It->second.LockLoc, "mutex locked here", DiagnosticIDs::Note);
      }

      // Remove from tracking after unlock
      MutexLocks.erase(It);
    }
  }
}

void MutexUnlockThreadCheck::onEndOfTranslationUnit() {
  // Check for mutexes that were locked but never unlocked
  for (const auto &Entry : MutexLocks) {
    diag(Entry.second.LockLoc,
         "mutex '%0' locked in function '%1' but not unlocked in the same translation unit; "
         "ensure proper unlock (MISRA C:2025 Rule 22.17)")
        << Entry.first << Entry.second.FunctionName;
  }

  MutexLocks.clear();
  SM = nullptr;
}

} // namespace clang::tidy::automotive

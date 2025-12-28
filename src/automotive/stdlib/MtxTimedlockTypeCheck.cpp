//===--- MtxTimedlockTypeCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MtxTimedlockTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

// Mutex type constants from <threads.h>
// mtx_plain = 0x1
// mtx_recursive = 0x2
// mtx_timed = 0x4
static const int MTX_PLAIN = 0x1;
static const int MTX_RECURSIVE = 0x2;
static const int MTX_TIMED = 0x4;

void MtxTimedlockTypeCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to mtx_init to track mutex types
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("mtx_init")))).bind("mtxInit"),
      this);

  // Match calls to mtx_timedlock
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("mtx_timedlock"))))
          .bind("mtxTimedlock"),
      this);
}

int MtxTimedlockTypeCheck::getMutexTypeFromInit(
    const CallExpr *InitCall, ASTContext *Ctx) const {
  if (InitCall->getNumArgs() < 2)
    return -1;

  const Expr *TypeArg = InitCall->getArg(1)->IgnoreParenImpCasts();

  // Try to evaluate the type argument as a constant
  Expr::EvalResult Result;
  if (!TypeArg->EvaluateAsInt(Result, *Ctx))
    return -1;

  return Result.Val.getInt().getExtValue();
}

bool MtxTimedlockTypeCheck::supportsTimeout(int MutexType) const {
  // A mutex supports timeout if it has the mtx_timed flag set
  // Valid types: mtx_timed or (mtx_timed | mtx_recursive)
  return (MutexType & MTX_TIMED) != 0;
}

void MtxTimedlockTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const SourceManager &SM = *Result.SourceManager;

  // Handle mtx_init calls to track mutex types
  if (const auto *InitCall = Result.Nodes.getNodeAs<CallExpr>("mtxInit")) {
    if (SM.isInSystemHeader(InitCall->getBeginLoc()))
      return;

    if (InitCall->getNumArgs() < 2)
      return;

    const Expr *MutexArg = InitCall->getArg(0)->IgnoreParenImpCasts();

    // Get the mutex variable
    const VarDecl *MutexVar = nullptr;
    if (const auto *UO = dyn_cast<UnaryOperator>(MutexArg)) {
      if (UO->getOpcode() == UO_AddrOf) {
        if (const auto *DRE =
                dyn_cast<DeclRefExpr>(UO->getSubExpr()->IgnoreParenImpCasts())) {
          MutexVar = dyn_cast<VarDecl>(DRE->getDecl());
        }
      }
    }

    if (!MutexVar)
      return;

    int MutexType = getMutexTypeFromInit(InitCall, Result.Context);
    if (MutexType != -1) {
      MutexTypes[MutexVar] = MutexType;
    }

    return;
  }

  // Handle mtx_timedlock calls
  if (const auto *TimedlockCall =
          Result.Nodes.getNodeAs<CallExpr>("mtxTimedlock")) {
    if (SM.isInSystemHeader(TimedlockCall->getBeginLoc()))
      return;

    if (TimedlockCall->getNumArgs() < 2)
      return;

    const Expr *MutexArg = TimedlockCall->getArg(0)->IgnoreParenImpCasts();

    // Get the mutex variable
    const VarDecl *MutexVar = nullptr;
    if (const auto *UO = dyn_cast<UnaryOperator>(MutexArg)) {
      if (UO->getOpcode() == UO_AddrOf) {
        if (const auto *DRE =
                dyn_cast<DeclRefExpr>(UO->getSubExpr()->IgnoreParenImpCasts())) {
          MutexVar = dyn_cast<VarDecl>(DRE->getDecl());
        }
      }
    } else if (const auto *DRE = dyn_cast<DeclRefExpr>(MutexArg)) {
      // Direct reference (already a pointer)
      MutexVar = dyn_cast<VarDecl>(DRE->getDecl());
    }

    if (!MutexVar)
      return;

    // Check if we know the mutex type from a previous mtx_init call
    auto It = MutexTypes.find(MutexVar);
    if (It != MutexTypes.end()) {
      int MutexType = It->second;
      if (!supportsTimeout(MutexType)) {
        diag(TimedlockCall->getBeginLoc(),
             "mtx_timedlock called on mutex '%0' that does not support "
             "timeout; mutex must be initialized with mtx_timed or "
             "(mtx_timed | mtx_recursive)")
            << MutexVar->getName();

        // Find the mtx_init call location for a note
        // This would require storing the location during init tracking
        diag(MutexVar->getLocation(),
             "mutex declared here; initialized without mtx_timed flag",
             DiagnosticIDs::Note);
      }
    } else {
      // We don't know the mutex type - warn that it should be verifiable
      diag(TimedlockCall->getBeginLoc(),
           "mtx_timedlock called on mutex '%0' whose type cannot be verified; "
           "ensure mutex is initialized with mtx_timed or (mtx_timed | "
           "mtx_recursive)")
          << MutexVar->getName();
    }
  }
}

void MtxTimedlockTypeCheck::onEndOfTranslationUnit() {
  // Clear the tracked mutex types for the next translation unit
  MutexTypes.clear();
}

} // namespace clang::tidy::automotive

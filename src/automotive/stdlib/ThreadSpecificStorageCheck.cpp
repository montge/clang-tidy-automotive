//===--- ThreadSpecificStorageCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ThreadSpecificStorageCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ThreadSpecificStorageCheck::registerMatchers(MatchFinder *Finder) {
  // Match tss_create, pthread_key_create (creation)
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("tss_create", "pthread_key_create"))))
          .bind("createTSS"),
      this);

  // Match tss_get, pthread_getspecific (retrieval)
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("tss_get", "pthread_getspecific"))))
          .bind("getTSS"),
      this);

  // Match tss_set, pthread_setspecific (setting)
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("tss_set", "pthread_setspecific"))))
          .bind("setTSS"),
      this);
}

static std::string getKeyName(const Expr *E) {
  E = E->IgnoreParenImpCasts();

  // Handle &key
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

void ThreadSpecificStorageCheck::check(
    const MatchFinder::MatchResult &Result) {
  SM = Result.SourceManager;

  // Handle TSS creation
  if (const auto *CreateCall = Result.Nodes.getNodeAs<CallExpr>("createTSS")) {
    if (SM->isInSystemHeader(CreateCall->getBeginLoc()))
      return;

    if (CreateCall->getNumArgs() < 1)
      return;

    std::string KeyName = getKeyName(CreateCall->getArg(0));
    if (KeyName.empty())
      return;

    // Record the creation location
    // Note: We can't easily track thread context in static analysis
    TSSKeys[KeyName] = {CreateCall->getBeginLoc(), ""};
  }

  // Handle TSS retrieval
  if (const auto *GetCall = Result.Nodes.getNodeAs<CallExpr>("getTSS")) {
    if (SM->isInSystemHeader(GetCall->getBeginLoc()))
      return;

    if (GetCall->getNumArgs() < 1)
      return;

    std::string KeyName = getKeyName(GetCall->getArg(0));
    if (KeyName.empty())
      return;

    // Warn about potential cross-thread access
    // This is a best-effort warning since we can't track thread IDs statically
    diag(GetCall->getBeginLoc(),
         "thread-specific storage key '%0' retrieved; ensure this is called "
         "from the same thread that set the value (MISRA C:2025 Rule 22.15)")
        << KeyName;

    // Note: A more sophisticated analysis would track thread creation
    // and enforce that tss_get is only called from threads that called tss_set
    // However, this requires complex inter-procedural and inter-thread analysis
  }

  // Handle TSS setting
  if (const auto *SetCall = Result.Nodes.getNodeAs<CallExpr>("setTSS")) {
    if (SM->isInSystemHeader(SetCall->getBeginLoc()))
      return;

    if (SetCall->getNumArgs() < 1)
      return;

    std::string KeyName = getKeyName(SetCall->getArg(0));
    if (KeyName.empty())
      return;

    // Track that this key has been set
    // In a real implementation, we'd track which thread set it
  }
}

void ThreadSpecificStorageCheck::onEndOfTranslationUnit() {
  TSSKeys.clear();
  SM = nullptr;
}

} // namespace clang::tidy::automotive

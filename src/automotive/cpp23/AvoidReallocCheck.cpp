//===--- AvoidReallocCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidReallocCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidReallocCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to realloc function
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("realloc")))).bind("call"), this);

  // Also match std::realloc
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("::std::realloc")))).bind("call"),
      this);
}

void AvoidReallocCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  if (!Call)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  diag(Call->getBeginLoc(),
       "use of realloc is prohibited; it can cause memory leaks and "
       "undefined behavior");
}

} // namespace clang::tidy::automotive

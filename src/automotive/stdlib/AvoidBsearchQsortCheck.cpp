//===--- AvoidBsearchQsortCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidBsearchQsortCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidBsearchQsortCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("bsearch", "qsort"))))
          .bind("call"),
      this);
}

void AvoidBsearchQsortCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  if (!Call)
    return;

  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  const FunctionDecl *FD = Call->getDirectCallee();
  if (!FD)
    return;

  StringRef FuncName = FD->getName();
  diag(Call->getBeginLoc(), "%0 shall not be used") << FuncName;
}

} // namespace clang::tidy::automotive

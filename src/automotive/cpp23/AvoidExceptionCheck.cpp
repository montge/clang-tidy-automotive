//===--- AvoidExceptionCheck.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidExceptionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidExceptionCheck::registerMatchers(MatchFinder *Finder) {
  // Match try statements
  Finder->addMatcher(cxxTryStmt().bind("try"), this);

  // Match throw expressions
  Finder->addMatcher(cxxThrowExpr().bind("throw"), this);

  // Match catch handlers
  Finder->addMatcher(cxxCatchStmt().bind("catch"), this);
}

void AvoidExceptionCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Try = Result.Nodes.getNodeAs<CXXTryStmt>("try")) {
    if (Result.SourceManager->isInSystemHeader(Try->getTryLoc()))
      return;
    diag(Try->getTryLoc(), "exception handling shall not be used");
  } else if (const auto *Throw =
                 Result.Nodes.getNodeAs<CXXThrowExpr>("throw")) {
    if (Result.SourceManager->isInSystemHeader(Throw->getThrowLoc()))
      return;
    diag(Throw->getThrowLoc(), "exception handling shall not be used");
  } else if (const auto *Catch =
                 Result.Nodes.getNodeAs<CXXCatchStmt>("catch")) {
    if (Result.SourceManager->isInSystemHeader(Catch->getCatchLoc()))
      return;
    diag(Catch->getCatchLoc(), "exception handling shall not be used");
  }
}

} // namespace clang::tidy::automotive

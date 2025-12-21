//===--- AvoidDynamicMemoryCppCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidDynamicMemoryCppCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidDynamicMemoryCppCheck::registerMatchers(MatchFinder *Finder) {
  // Match new expressions
  Finder->addMatcher(cxxNewExpr().bind("new"), this);

  // Match delete expressions
  Finder->addMatcher(cxxDeleteExpr().bind("delete"), this);
}

void AvoidDynamicMemoryCppCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *New = Result.Nodes.getNodeAs<CXXNewExpr>("new")) {
    if (Result.SourceManager->isInSystemHeader(New->getBeginLoc()))
      return;
    diag(New->getBeginLoc(), "dynamic memory allocation shall not be used");
  } else if (const auto *Delete =
                 Result.Nodes.getNodeAs<CXXDeleteExpr>("delete")) {
    if (Result.SourceManager->isInSystemHeader(Delete->getBeginLoc()))
      return;
    diag(Delete->getBeginLoc(),
         "dynamic memory deallocation shall not be used");
  }
}

} // namespace clang::tidy::automotive

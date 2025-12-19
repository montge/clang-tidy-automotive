//===--- AvoidDynamicCastCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidDynamicCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidDynamicCastCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match dynamic_cast expressions
  Finder->addMatcher(cxxDynamicCastExpr().bind("cast"), this);
}

void AvoidDynamicCastCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<CXXDynamicCastExpr>("cast");
  if (!Cast)
    return;

  // Skip casts in system headers
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  diag(Cast->getBeginLoc(),
       "dynamic_cast should not be used; prefer static polymorphism or "
       "static_cast with prior type checking");
}

} // namespace clang::tidy::automotive

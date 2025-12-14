//===--- AvoidCStyleCastCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCStyleCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidCStyleCastCheck::registerMatchers(MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match C-style casts
  Finder->addMatcher(cStyleCastExpr().bind("cast"), this);
}

void AvoidCStyleCastCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<CStyleCastExpr>("cast");
  if (!Cast)
    return;

  // Skip casts in system headers
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  // Skip implicit casts that appear as C-style in AST
  if (Cast->getBeginLoc().isMacroID())
    return;

  diag(Cast->getBeginLoc(),
       "C-style casts shall not be used; use static_cast, const_cast, "
       "reinterpret_cast, or dynamic_cast instead");
}

} // namespace clang::tidy::automotive

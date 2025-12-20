//===--- AvoidReinterpretCastCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidReinterpretCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidReinterpretCastCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxReinterpretCastExpr().bind("cast"), this);
}

void AvoidReinterpretCastCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<CXXReinterpretCastExpr>("cast");
  if (!Cast)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Cast->getOperatorLoc()))
    return;

  diag(Cast->getOperatorLoc(), "reinterpret_cast shall not be used");
}

} // namespace clang::tidy::automotive

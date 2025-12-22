//===--- AvoidStatementExpressionCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStatementExpressionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidStatementExpressionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(stmtExpr().bind("stmtexpr"), this);
}

void AvoidStatementExpressionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *SE = Result.Nodes.getNodeAs<StmtExpr>("stmtexpr");
  if (!SE)
    return;

  if (Result.SourceManager->isInSystemHeader(SE->getBeginLoc()))
    return;

  diag(SE->getBeginLoc(),
       "GNU statement expression extension shall not be used");
}

} // namespace clang::tidy::automotive::cpp23

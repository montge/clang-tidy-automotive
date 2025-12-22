//===--- StaticInlineCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StaticInlineCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclCXX.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void StaticInlineCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(allOf(isInline(), unless(isStaticStorageClass())))
          .bind("inline"),
      this);
}

void StaticInlineCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("inline");

  if (!MatchedDecl)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(MatchedDecl->getLocation()))
    return;

  // Skip C++ lambda call operators - lambdas cannot be declared static
  if (const auto *MD = dyn_cast<CXXMethodDecl>(MatchedDecl)) {
    if (MD->getParent()->isLambda())
      return;
  }

  // Skip class member functions - they have different rules
  if (isa<CXXMethodDecl>(MatchedDecl))
    return;

  diag(MatchedDecl->getBeginLoc(), "inline function shall be static")
      << FixItHint::CreateInsertion(MatchedDecl->getBeginLoc(), "static ");
}

} // namespace clang::tidy::automotive

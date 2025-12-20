//===--- AvoidThrowInNoexceptCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidThrowInNoexceptCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidThrowInNoexceptCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus11)
    return;

  // Match throw expressions inside noexcept functions
  Finder->addMatcher(
      cxxThrowExpr(hasAncestor(functionDecl(isNoThrow()).bind("func")))
          .bind("throw"),
      this);
}

void AvoidThrowInNoexceptCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Throw = Result.Nodes.getNodeAs<CXXThrowExpr>("throw");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

  if (!Throw || !Func)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Throw->getThrowLoc()))
    return;

  diag(Throw->getThrowLoc(),
       "throw expression in noexcept function %0 will call std::terminate if "
       "exception propagates")
      << Func;
}

} // namespace clang::tidy::automotive

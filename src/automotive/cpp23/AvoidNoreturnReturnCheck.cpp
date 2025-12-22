//===--- AvoidNoreturnReturnCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNoreturnReturnCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidNoreturnReturnCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus11)
    return;

  // Match return statements inside functions with noreturn attribute
  // [[noreturn]] uses CXX11NoReturn, _Noreturn uses NoReturn
  Finder->addMatcher(
      returnStmt(hasAncestor(functionDecl(anyOf(hasAttr(attr::NoReturn),
                                                hasAttr(attr::CXX11NoReturn)))
                                 .bind("func")))
          .bind("return"),
      this);
}

void AvoidNoreturnReturnCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Return = Result.Nodes.getNodeAs<ReturnStmt>("return");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

  // LCOV_EXCL_START - defensive check, matcher guarantees these exist
  if (!Return || !Func)
    return;
  // LCOV_EXCL_STOP

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Return->getBeginLoc()))
    return;

  diag(Return->getBeginLoc(),
       "return statement in noreturn function %0 violates the function's "
       "contract to never return")
      << Func;
}

} // namespace clang::tidy::automotive

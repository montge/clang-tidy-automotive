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
  // Match return statements in noreturn functions
  Finder->addMatcher(
      returnStmt(hasAncestor(functionDecl(isNoReturn()).bind("func")))
          .bind("return"),
      this);
}

void AvoidNoreturnReturnCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Return = Result.Nodes.getNodeAs<ReturnStmt>("return");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

  if (!Return || !Func)
    return;

  if (Result.SourceManager->isInSystemHeader(Return->getReturnLoc()))
    return;

  diag(Return->getReturnLoc(),
       "_Noreturn function '%0' shall not return to its caller")
      << Func->getName();
}

} // namespace clang::tidy::automotive

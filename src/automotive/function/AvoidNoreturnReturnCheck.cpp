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
  // Match noreturn function definitions
  Finder->addMatcher(
      functionDecl(isDefinition(), isNoReturn(),
                   unless(isExpansionInSystemHeader()))
          .bind("func"),
      this);
}

void AvoidNoreturnReturnCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!Func || !Func->getBody())
    return;

  // Find all return statements in the function body
  for (auto *S : Func->getBody()->children()) {
    if (auto *RS = dyn_cast<ReturnStmt>(S)) {
      diag(RS->getReturnLoc(),
           "_Noreturn function '%0' shall not return to its caller")
          << Func->getName();
    }
  }
}

} // namespace clang::tidy::automotive

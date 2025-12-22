//===--- MissingElseCheck.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingElseCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MissingElseCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      ifStmt(hasElse(ifStmt(unless(hasElse(anything()))).bind("ifStmt"))),
      this);
}

void MissingElseCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("ifStmt");

  if (MatchedDecl) {
    diag(MatchedDecl->getThen()->getEndLoc(),
         "if...else if chain should terminate with an else clause");
  }
}

} // namespace clang::tidy::automotive

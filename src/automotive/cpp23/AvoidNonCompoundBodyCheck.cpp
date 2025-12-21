//===--- AvoidNonCompoundBodyCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNonCompoundBodyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidNonCompoundBodyCheck::registerMatchers(MatchFinder *Finder) {
  // Match if statements with non-compound bodies
  Finder->addMatcher(ifStmt(hasThen(stmt(unless(compoundStmt())))).bind("if"),
                     this);

  // Match if statements with non-compound else bodies
  Finder->addMatcher(
      ifStmt(hasElse(stmt(unless(anyOf(compoundStmt(), ifStmt())))))
          .bind("else"),
      this);

  // Match while statements
  Finder->addMatcher(
      whileStmt(hasBody(stmt(unless(compoundStmt())))).bind("while"), this);

  // Match do-while statements
  Finder->addMatcher(doStmt(hasBody(stmt(unless(compoundStmt())))).bind("do"),
                     this);

  // Match for statements
  Finder->addMatcher(forStmt(hasBody(stmt(unless(compoundStmt())))).bind("for"),
                     this);

  // Match range-based for statements
  Finder->addMatcher(
      cxxForRangeStmt(hasBody(stmt(unless(compoundStmt())))).bind("range"),
      this);
}

void AvoidNonCompoundBodyCheck::check(const MatchFinder::MatchResult &Result) {
  SourceLocation Loc;
  const char *StmtType = nullptr;

  if (const auto *S = Result.Nodes.getNodeAs<IfStmt>("if")) {
    if (Result.SourceManager->isInSystemHeader(S->getIfLoc()))
      return;
    Loc = S->getIfLoc();
    StmtType = "if";
  } else if (const auto *S = Result.Nodes.getNodeAs<IfStmt>("else")) {
    if (Result.SourceManager->isInSystemHeader(S->getElseLoc()))
      return;
    Loc = S->getElseLoc();
    StmtType = "else";
  } else if (const auto *S = Result.Nodes.getNodeAs<WhileStmt>("while")) {
    if (Result.SourceManager->isInSystemHeader(S->getWhileLoc()))
      return;
    Loc = S->getWhileLoc();
    StmtType = "while";
  } else if (const auto *S = Result.Nodes.getNodeAs<DoStmt>("do")) {
    if (Result.SourceManager->isInSystemHeader(S->getDoLoc()))
      return;
    Loc = S->getDoLoc();
    StmtType = "do";
  } else if (const auto *S = Result.Nodes.getNodeAs<ForStmt>("for")) {
    if (Result.SourceManager->isInSystemHeader(S->getForLoc()))
      return;
    Loc = S->getForLoc();
    StmtType = "for";
  } else if (const auto *S = Result.Nodes.getNodeAs<CXXForRangeStmt>("range")) {
    if (Result.SourceManager->isInSystemHeader(S->getForLoc()))
      return;
    Loc = S->getForLoc();
    StmtType = "range-based for";
  } else {
    return;
  }

  diag(Loc, "%0 statement body shall be a compound statement") << StmtType;
}

} // namespace clang::tidy::automotive

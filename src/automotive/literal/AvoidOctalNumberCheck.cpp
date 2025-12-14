//===--- AvoidOctalNumberCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidOctalNumberCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Token.h"
#include "llvm/Support/Regex.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidOctalNumberCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(integerLiteral().bind("integer"), this);
}

void AvoidOctalNumberCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<IntegerLiteral>("integer");
  if (!MatchedDecl)
    return;

  const SourceManager &SM = *Result.SourceManager;
  const LangOptions &LangOpts = Result.Context->getLangOpts();
  const llvm::Regex RE("^0[0-9]+[luLU]?$");

  StringRef Token = Lexer::getSourceText(
      CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc(),
                                     MatchedDecl->getEndLoc()),
      SM, LangOpts);

  if (RE.match(Token))
    diag(MatchedDecl->getBeginLoc(), "Avoid octal number");
}

} // namespace clang::tidy::automotive

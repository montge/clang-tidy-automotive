//===--- AvoidUnsignedLiteralWithoutSuffixCheck.cpp - clang-tidy ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnsignedLiteralWithoutSuffixCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidUnsignedLiteralWithoutSuffixCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(integerLiteral().bind("literal"), this);
}

void AvoidUnsignedLiteralWithoutSuffixCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Literal = Result.Nodes.getNodeAs<IntegerLiteral>("literal");
  if (!Literal)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Literal->getLocation()))
    return;

  // Check if the type is unsigned
  QualType Type = Literal->getType();
  if (!Type->isUnsignedIntegerType())
    return;

  // Get the source text of the literal
  SourceLocation Loc = Literal->getLocation();
  SourceManager &SM = *Result.SourceManager;
  const LangOptions &LangOpts = Result.Context->getLangOpts();

  CharSourceRange Range = CharSourceRange::getTokenRange(Loc);
  StringRef LiteralText = Lexer::getSourceText(Range, SM, LangOpts);

  if (LiteralText.empty())
    return;

  // Check if it already has a 'u' or 'U' suffix
  bool HasUnsignedSuffix = false;
  for (char C : LiteralText) {
    if (C == 'u' || C == 'U') {
      HasUnsignedSuffix = true;
      break;
    }
  }

  if (HasUnsignedSuffix)
    return;

  // Only warn for hex or octal literals that become unsigned due to their value
  // Decimal literals less than INT_MAX are always signed
  bool IsHexOrOctal = LiteralText.starts_with("0x") ||
                      LiteralText.starts_with("0X") ||
                      (LiteralText.starts_with("0") && LiteralText.size() > 1 &&
                       LiteralText[1] >= '0' && LiteralText[1] <= '7');

  if (!IsHexOrOctal)
    return;

  diag(Literal->getLocation(),
       "unsigned integer constant should have a 'U' suffix");
}

} // namespace clang::tidy::automotive

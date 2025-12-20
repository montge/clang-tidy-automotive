//===--- UnterminatedEscapeSequenceCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnterminatedEscapeSequenceCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/Support/Regex.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void UnterminatedEscapeSequenceCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(stringLiteral().bind("stringLiteral"), this);
  Finder->addMatcher(characterLiteral().bind("charLiteral"), this);
}

void UnterminatedEscapeSequenceCheck::check(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  const auto *MatchString =
      Result.Nodes.getNodeAs<StringLiteral>("stringLiteral");
  if (MatchString) {
    // Only check narrow character strings (char width 1)
    // Wide strings (wchar_t, char16_t, char32_t) are not supported
    if (MatchString->getCharByteWidth() != 1)
      return;
    checkEscapeSequences(MatchString->getEndLoc(), MatchString->getBeginLoc(),
                         MatchString->getString());
    return;
  }

  const auto *MatchChar =
      Result.Nodes.getNodeAs<CharacterLiteral>("charLiteral");
  if (MatchChar) {
    char Value = MatchChar->getValue();
    checkEscapeSequences(MatchChar->getBeginLoc(), MatchChar->getEndLoc(),
                         StringRef(&Value, 1));
    return;
  }
}

void UnterminatedEscapeSequenceCheck::checkEscapeSequences(
    SourceLocation StartLoc, SourceLocation EndLoc, StringRef Str) {
  diag(StartLoc, "'%0'") << Str;
}

} // namespace clang::tidy::automotive

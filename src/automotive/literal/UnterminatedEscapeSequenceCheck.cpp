//===--- UnterminatedEscapeSequenceCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// MISRA C:2025 Rule 4.1 - Octal and hexadecimal escape sequences shall be
// terminated. This check detects unterminated or invalid escape sequences
// in string and character literals.
//
//===----------------------------------------------------------------------===//

#include "UnterminatedEscapeSequenceCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void UnterminatedEscapeSequenceCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(stringLiteral().bind("stringLiteral"), this);
}

void UnterminatedEscapeSequenceCheck::check(
    const ast_matchers::MatchFinder::MatchResult &Result) {

  const auto *Literal = Result.Nodes.getNodeAs<StringLiteral>("stringLiteral");
  if (!Literal)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Literal->getBeginLoc()))
    return;

  // Only check ordinary string literals (not wide, UTF-8, etc.)
  if (Literal->getKind() != StringLiteralKind::Ordinary)
    return;

  // Get the source text of the literal to analyze escape sequences
  SourceLocation BeginLoc = Literal->getBeginLoc();
  SourceLocation EndLoc = Literal->getEndLoc();

  // Get the actual source text including quotes
  CharSourceRange Range = CharSourceRange::getTokenRange(BeginLoc, EndLoc);
  StringRef SourceText = Lexer::getSourceText(Range, *Result.SourceManager,
                                              Result.Context->getLangOpts());

  if (SourceText.empty())
    return;

  // Analyze the source text for problematic escape sequences
  // Look for octal escapes followed by digits (ambiguous/unterminated)
  // and hex escapes that might be ambiguous
  checkSourceForProblematicEscapes(BeginLoc, SourceText, *Result.SourceManager);
}

void UnterminatedEscapeSequenceCheck::checkSourceForProblematicEscapes(
    SourceLocation Loc, StringRef Source, const SourceManager &SM) {

  // Skip the opening quote
  if (Source.empty() || Source.front() != '"')
    return;

  size_t i = 1; // Start after opening quote
  size_t len = Source.size();

  // Don't process the closing quote
  if (len > 0 && Source.back() == '"')
    len--;

  while (i < len) {
    if (Source[i] == '\\' && i + 1 < len) {
      char next = Source[i + 1];

      // Check for octal escape sequences followed by more digits
      // This creates ambiguity: \0129 - is it \012 followed by 9, or \0
      // followed by 129?
      if (next >= '0' && next <= '7') {
        // Found start of octal escape
        size_t octalStart = i + 1;
        size_t octalEnd = octalStart;

        // Count octal digits (max 3)
        while (octalEnd < len && octalEnd < octalStart + 3 &&
               Source[octalEnd] >= '0' && Source[octalEnd] <= '7') {
          octalEnd++;
        }

        // Check if followed by another digit (ambiguous)
        if (octalEnd < len && Source[octalEnd] >= '0' &&
            Source[octalEnd] <= '9') {
          diag(Loc, "octal escape sequence '\\%0' is followed by digit '%1'; "
                    "consider using hex escape or separating strings")
              << Source.substr(octalStart, octalEnd - octalStart)
              << Source.substr(octalEnd, 1);
        }

        i = octalEnd;
        continue;
      }

      // Check for hex escape sequences that might be too long
      if (next == 'x') {
        size_t hexStart = i + 2;
        size_t hexEnd = hexStart;

        // Count hex digits
        while (hexEnd < len &&
               ((Source[hexEnd] >= '0' && Source[hexEnd] <= '9') ||
                (Source[hexEnd] >= 'a' && Source[hexEnd] <= 'f') ||
                (Source[hexEnd] >= 'A' && Source[hexEnd] <= 'F'))) {
          hexEnd++;
        }

        // Warn if hex escape has more than 2 digits (may overflow char)
        if (hexEnd - hexStart > 2) {
          diag(Loc, "hex escape sequence '\\x%0' has %1 digits; "
                    "only the last 2 hex digits fit in a char")
              << Source.substr(hexStart, hexEnd - hexStart)
              << (hexEnd - hexStart);
        }

        i = hexEnd;
        continue;
      }

      // Skip valid simple escapes
      if (next == 'n' || next == 't' || next == 'r' || next == '\\' ||
          next == '\'' || next == '"' || next == 'a' || next == 'b' ||
          next == 'f' || next == 'v' || next == '?' || next == '0') {
        i += 2;
        continue;
      }

      // Unknown escape sequences are already caught by the compiler
      // (-Wunknown-escape-sequence), so we don't need to flag them here
      i += 2;
    } else {
      i++;
    }
  }
}

} // namespace clang::tidy::automotive

//===--- UnterminatedEscapeSequenceCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNTERMINATEDESCAPESEQUENCECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNTERMINATEDESCAPESEQUENCECHECK_H

#include "../../ClangTidyCheck.h"
#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/StringRef.h"

namespace clang::tidy::automotive {

/// Detects unterminated or invalid escape sequences in string literals.
///
/// Escape sequences in strings and character constants must be well-formed.
/// Invalid escape sequences have implementation-defined behavior and can lead
/// to portability issues. Common errors include incomplete octal or hexadecimal
/// escape sequences that may silently produce unexpected characters.
///
/// Related MISRA C:2025 Rule: 4.1 - Escape sequences shall be well-formed.
///
/// Example:
/// \code
///   char str[] = "\x";    // Warning: incomplete hex escape sequence
///   char ch = '\77';      // Warning: incomplete octal escape
/// \endcode
class UnterminatedEscapeSequenceCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  UnterminatedEscapeSequenceCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~UnterminatedEscapeSequenceCheck() override = default;

  /// Registers AST matchers for string and character literals.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched literals and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Checks source text for problematic escape sequences.
  /// Detects octal escapes followed by digits (ambiguous) and
  /// overly long hex escapes that may overflow.
  /// \param Loc The location for diagnostics.
  /// \param Source The raw source text of the string literal.
  /// \param SM The source manager.
  void checkSourceForProblematicEscapes(SourceLocation Loc, StringRef Source,
                                        const SourceManager &SM);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNTERMINATEDESCAPESEQUENCECHECK_H

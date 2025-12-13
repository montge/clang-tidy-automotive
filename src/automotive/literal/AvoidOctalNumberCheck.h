//===--- AvoidOctalNumberCheck.h - clang-tidy --------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOCTALNUMBERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOCTALNUMBERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects octal literal constants (except zero).
///
/// Octal literals are denoted by a leading zero, which can be easily
/// overlooked or added accidentally. This can lead to confusion where a
/// developer intends a decimal value but accidentally creates an octal
/// constant, resulting in unexpected values.
///
/// Related MISRA C:2025 Rule: 7.1 - Octal constants shall not be used.
///
/// Example:
/// \code
///   int x = 0123;  // Warning: octal literal (decimal 83)
///   int y = 0;     // OK: zero is allowed
/// \endcode
class AvoidOctalNumberCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidOctalNumberCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for integer literals.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched octal literals and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOCTALNUMBERCHECK_H

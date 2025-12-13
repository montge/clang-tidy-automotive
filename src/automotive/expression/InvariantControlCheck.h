//===--- InvariantControlCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_INVARIANTCONTROLCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_INVARIANTCONTROLCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects control expressions with invariant values.
///
/// Loop control expressions and conditional statements should depend on
/// variable values that can change. An invariant (constant) control expression
/// results in an infinite loop or dead code, which is usually a programming
/// error that indicates a logic mistake or incomplete implementation.
///
/// Related MISRA C:2025 Rule: 14.2 - A for loop shall be well-formed.
///
/// Example:
/// \code
///   while (1) { }       // Warning: invariant control expression
///   for (;;) { }        // Warning: invariant control expression
/// \endcode
class InvariantControlCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  InvariantControlCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for loop and conditional statements.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched control expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_INVARIANTCONTROLCHECK_H

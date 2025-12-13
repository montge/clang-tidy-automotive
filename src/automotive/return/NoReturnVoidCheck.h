//===--- NoReturnVoidCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_NORETURNVOIDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_NORETURNVOIDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects void functions with return statements that have expressions.
///
/// A function with void return type should not return a value. Using return
/// with an expression in a void function is either a mistake or indicates
/// confusion about the function's contract. All void function returns should
/// be simple "return;" statements without values.
///
/// Related MISRA C:2025 Rule: 15.6 - The body of an iteration-statement or a
/// selection-statement shall be a compound-statement.
///
/// Example:
/// \code
///   void func(void) {
///     return 0;  // Warning: returning value from void function
///   }
/// \endcode
class NoReturnVoidCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  NoReturnVoidCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for return statements in void functions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched return statements and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_NORETURNVOIDCHECK_H

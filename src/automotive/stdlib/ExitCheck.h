//===--- ExitCheck.h - clang-tidy ------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXITCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXITCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of exit(), abort(), and _Exit() functions.
///
/// These functions cause abrupt program termination without returning to
/// the caller, bypassing normal cleanup and resource deallocation. In
/// safety-critical and embedded systems, controlled shutdown is essential,
/// and abrupt termination can leave the system in an undefined state.
///
/// Related MISRA C:2025 Rule: 21.8 - The library functions abort, exit,
/// and _Exit shall not be used.
///
/// Example:
/// \code
///   exit(1);    // Warning: exit() usage
///   abort();    // Warning: abort() usage
/// \endcode
class ExitCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  ExitCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for exit/abort calls.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched exit/abort calls and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXITCHECK_H

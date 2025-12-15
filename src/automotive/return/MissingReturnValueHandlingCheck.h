//===--- MissingReturnValueHandlingCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGRETURNVALUEHANDLINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGRETURNVALUEHANDLINGCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects function calls where the return value is ignored.
///
/// When a function returns a value, that value should be used or explicitly
/// discarded. Ignoring return values, especially error codes or status
/// indicators, can lead to unchecked error conditions and program failures.
/// This is particularly important for functions that return error status.
///
/// Related MISRA C:2025 Rule: 17.7 - The value returned by a function having
/// non-void return type shall be used.
///
/// Example:
/// \code
///   int getStatus(void);
///   getStatus();  // Warning: return value ignored
/// \endcode
class MissingReturnValueHandlingCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  MissingReturnValueHandlingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~MissingReturnValueHandlingCheck() override = default;

  /// Registers AST matchers for function calls.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched function calls and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGRETURNVALUEHANDLINGCHECK_H

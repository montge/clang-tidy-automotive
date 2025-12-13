//===--- AvoidFunctionParameterModificationCheck.h - clang-tidy -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFUNCTIONPARAMETERMODIFICATIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFUNCTIONPARAMETERMODIFICATIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects modifications to function parameters within the function body.
///
/// Modifying function parameters can make code harder to understand and
/// debug, as the parameter value changes during execution. This can lead to
/// confusion about the original input value and makes the function's behavior
/// less predictable. Using local copies for modifications is clearer.
///
/// Related MISRA C:2025 Rule: 17.8 - A function parameter should not be
/// modified.
///
/// Example:
/// \code
///   void func(int x) {
///     x = x + 1;  // Warning: modifying parameter
///   }
/// \endcode
class AvoidFunctionParameterModificationCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidFunctionParameterModificationCheck(StringRef Name,
                                          ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for parameter modifications.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched parameter modifications and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFUNCTIONPARAMETERMODIFICATIONCHECK_H

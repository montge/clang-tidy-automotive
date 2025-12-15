//===--- AvoidCommaOperatorCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMAOPERATORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMAOPERATORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of the comma operator outside of for loop initialization.
///
/// The comma operator evaluates multiple expressions sequentially and returns
/// the result of the last expression. This can obscure the program's control
/// flow and make code harder to understand and maintain. Side effects from
/// earlier expressions in a comma sequence may be overlooked during code
/// review.
///
/// Related MISRA C:2025 Rule: 12.3 - The comma operator should not be used.
///
/// Example:
/// \code
///   int x = (a = 1, b = 2, a + b);  // Warning: comma operator used
/// \endcode
class AvoidCommaOperatorCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidCommaOperatorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidCommaOperatorCheck() override = default;

  /// Registers AST matchers for comma operator usage.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched comma operators and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMMAOPERATORCHECK_H

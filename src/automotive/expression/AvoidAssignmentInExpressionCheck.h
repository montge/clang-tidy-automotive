//===--- AvoidAssignmentInExpressionCheck.h - clang-tidy --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASSIGNMENTINEXPRESSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASSIGNMENTINEXPRESSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects assignment operations embedded within larger expressions.
///
/// Assignment within expressions can be easily confused with equality
/// comparison (== vs =), leading to subtle bugs. Separating assignment from
/// other operations makes code more readable and reduces the risk of
/// accidental assignment in conditional expressions.
///
/// Related MISRA C:2025 Rule: 13.4 - The result of an assignment operator
/// should not be used.
///
/// Example:
/// \code
///   if (x = getValue()) { }  // Warning: assignment in condition
///   int y = (z = 10) + 5;    // Warning: assignment in expression
/// \endcode
class AvoidAssignmentInExpressionCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidAssignmentInExpressionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidAssignmentInExpressionCheck() override = default;

  /// Registers AST matchers for assignment expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched assignments and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASSIGNMENTINEXPRESSIONCHECK_H

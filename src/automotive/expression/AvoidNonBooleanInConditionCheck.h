//===--- AvoidNonBooleanInConditionCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONBOOLEANINCONDITIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONBOOLEANINCONDITIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects non-boolean expressions used in conditional contexts.
///
/// Conditional expressions in if, while, for, and do-while statements should
/// have boolean type. Using integer or pointer types in conditions relies on
/// implicit conversion to boolean, which can obscure intent and make code
/// less clear. Explicit comparisons improve readability.
///
/// Related MISRA C:2025 Rule: 14.4 - The controlling expression of an if
/// statement and the controlling expression of an iteration-statement shall
/// have essentially Boolean type.
///
/// Example:
/// \code
///   int x;
///   if (x) { }        // Warning: non-boolean in condition
///   while (ptr) { }   // Warning: non-boolean in condition
/// \endcode
class AvoidNonBooleanInConditionCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidNonBooleanInConditionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for conditional expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched conditions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONBOOLEANINCONDITIONCHECK_H

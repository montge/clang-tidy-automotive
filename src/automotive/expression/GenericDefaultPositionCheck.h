//===--- GenericDefaultPositionCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICDEFAULTPOSITIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICDEFAULTPOSITIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selections where the default association is not first or
/// last.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. A generic selection consists of a controlling
/// expression or type and a list of associations, each mapping a type to an
/// expression. An optional default association provides a fallback when no type
/// matches.
///
/// For clarity and consistency, the default association should appear either as
/// the first or last association in the list. Placing it in the middle reduces
/// readability and makes it harder to identify the fallback case at a glance.
///
/// Related MISRA C:2025 Rule: 23.8 - A default association shall appear first
/// or last in a generic selection (Required).
///
/// Example:
/// \code
///   // Non-compliant: default in middle
///   _Generic(x, int: 1, default: 2, float: 3)
///
///   // Compliant: default first
///   _Generic(x, default: 0, int: 1, float: 2)
///
///   // Compliant: default last
///   _Generic(x, int: 1, float: 2, default: 0)
/// \endcode
class GenericDefaultPositionCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericDefaultPositionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericDefaultPositionCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICDEFAULTPOSITIONCHECK_H

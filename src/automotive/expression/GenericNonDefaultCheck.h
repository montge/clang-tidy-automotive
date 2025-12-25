//===--- GenericNonDefaultCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICNONDEFAULTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICNONDEFAULTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selections that contain only a default association.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. A generic selection consists of a controlling
/// expression and a list of associations, each mapping a type to an expression.
/// An optional default association provides a fallback when no type matches.
///
/// A generic selection should contain at least one non-default association to
/// provide meaningful type-specific behavior. A generic selection with only a
/// default association provides no type-specific selection and should be
/// replaced with the expression directly.
///
/// Related MISRA C:2025 Rule: 23.3 - A generic selection should contain at
/// least one non-default association (Advisory).
///
/// Example:
/// \code
///   // Non-compliant: only default association
///   _Generic(x, default: 42)
///
///   // Compliant: at least one non-default association
///   _Generic(x, int: 1, default: 0)
///
///   // Compliant: no default association
///   _Generic(x, int: 1, float: 2)
/// \endcode
class GenericNonDefaultCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericNonDefaultCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericNonDefaultCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICNONDEFAULTCHECK_H

//===--- GenericNoExplicitDefaultCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICNOEXPLICITDEFAULTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICNOEXPLICITDEFAULTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selections with explicit default associations.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. A generic selection consists of a controlling
/// expression and a list of associations, each mapping a type to an expression.
/// An optional default association provides a fallback when no type matches.
///
/// This check enforces that generic selections should not include an explicit
/// 'default:' association. Instead, all possible types should be explicitly
/// enumerated. This promotes type safety and ensures that all type cases are
/// deliberately handled.
///
/// Using explicit default associations can hide missing type cases and make
/// it harder to detect when new types need to be added to the generic
/// selection.
///
/// Related MISRA C:2025 Rule: 23.1 - A generic selection should not have an
/// explicit default association (Advisory).
///
/// Example:
/// \code
///   // Non-compliant: has explicit default
///   _Generic(x, int: 1, float: 2, default: 0)
///
///   // Compliant: all types explicitly listed
///   _Generic(x, int: 1, float: 2, double: 3)
/// \endcode
class GenericNoExplicitDefaultCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericNoExplicitDefaultCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericNoExplicitDefaultCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICNOEXPLICITDEFAULTCHECK_H

//===--- GenericFromMacroCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICFROMMACROCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICFROMMACROCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selections (_Generic) that are not expanded from a macro.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. To promote code clarity, maintainability, and
/// reusability, generic selections should be wrapped in macros rather than
/// used directly in source code.
///
/// Using _Generic directly in code can lead to:
/// - Reduced readability due to verbose syntax
/// - Code duplication when the same pattern is needed in multiple places
/// - Maintenance difficulties when the selection logic needs to be updated
///
/// Wrapping _Generic in a macro provides a clean interface and allows the
/// complexity to be abstracted away from the calling code.
///
/// Related MISRA C:2025 Rule: 23.1 - A generic selection should only be
/// expanded from a macro (Advisory).
///
/// Example:
/// \code
///   // Non-compliant: _Generic used directly
///   int x = 5;
///   int result = _Generic(x, int: 1, float: 2, default: 0);
///
///   // Compliant: _Generic wrapped in macro
///   #define TYPE_ID(x) _Generic((x), int: 1, float: 2, default: 0)
///   int result = TYPE_ID(x);
/// \endcode
class GenericFromMacroCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericFromMacroCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericFromMacroCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICFROMMACROCHECK_H

//===--- GenericSuitableControllingExprCheck.h - clang-tidy ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICSUITABLECONTROLLINGEXPRCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICSUITABLECONTROLLINGEXPRCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selections with unsuitable controlling expressions.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. The controlling expression determines which
/// association is selected based on its type. However, the controlling
/// expression itself is never evaluated - only its type is used for selection.
///
/// This check enforces that the controlling expression should have a suitable
/// type for generic selection. Unsuitable types include:
/// - void type (no value to select on)
/// - Function types (should use pointer-to-function)
/// - Incomplete types (type is not fully defined)
/// - Variably modified types (VLAs with non-constant bounds)
///
/// Using an unsuitable controlling expression can lead to unexpected behavior
/// or compilation errors.
///
/// Related MISRA C:2025 Rule: 23.4 - The controlling expression of a generic
/// selection shall be suitable (Required).
///
/// Example:
/// \code
///   void func(void);
///   extern int incomplete_array[];
///
///   // Non-compliant: void type
///   _Generic((void)0, default: 1)
///
///   // Non-compliant: function type
///   _Generic(func, default: 1)
///
///   // Non-compliant: incomplete array
///   _Generic(incomplete_array, default: 1)
///
///   // Compliant: complete object type
///   int x;
///   _Generic(x, int: 1, float: 2)
/// \endcode
class GenericSuitableControllingExprCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericSuitableControllingExprCheck(StringRef Name,
                                      ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericSuitableControllingExprCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICSUITABLECONTROLLINGEXPRCHECK_H

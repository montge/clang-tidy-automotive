//===--- GenericImplicitPointerCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICIMPLICITPOINTERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICIMPLICITPOINTERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selections that depend on implicit pointer type conversions.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. The controlling expression determines which
/// association is selected based on its type. However, when the controlling
/// expression undergoes implicit pointer conversions (such as array-to-pointer
/// decay, pointer-to-void conversions, or null pointer conversions), this can
/// lead to unexpected behavior where the selected association differs from what
/// the programmer intended.
///
/// This check warns when a generic selection's controlling expression contains
/// implicit pointer conversions that could affect which association is
/// selected, particularly:
/// - Array-to-pointer decay
/// - Pointer-to-void conversions
/// - Conversions between pointer types
/// - Null pointer conversions
///
/// Related MISRA C:2025 Rule: 23.5 - A generic selection should not depend on
/// implicit pointer type conversion (Advisory).
///
/// Example:
/// \code
///   int arr[5];
///   // Non-compliant: array-to-pointer decay affects type selection
///   _Generic(arr, int*: 1, int[5]: 2)
///
///   void *ptr;
///   // Non-compliant: void* conversion
///   _Generic(ptr, void*: 1, int*: 2)
///
///   // Compliant: explicit pointer type, no implicit conversion
///   int *p = &arr[0];
///   _Generic(p, int*: 1, float*: 2)
///
///   // Compliant: direct type matching
///   int x = 5;
///   _Generic(x, int: 1, float: 2)
/// \endcode
class GenericImplicitPointerCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericImplicitPointerCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericImplicitPointerCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Checks if the expression contains problematic implicit pointer
  /// conversions.
  /// \param Expr The expression to analyze.
  /// \return True if problematic implicit pointer conversions are found.
  bool hasImplicitPointerConversion(const Expr *Expr) const;

  /// Checks if a cast expression represents a problematic pointer conversion.
  /// \param Cast The implicit cast expression to check.
  /// \return True if the cast is a problematic pointer conversion.
  bool isProblematicPointerCast(const ImplicitCastExpr *Cast) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICIMPLICITPOINTERCHECK_H

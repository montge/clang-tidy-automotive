//===--- ExplicitEnumeratorValuesCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXPLICITENUMERATORVALUESCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXPLICITENUMERATORVALUESCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects enumeration constants without explicit values.
///
/// MISRA C:2025 Rule 9.2 requires that each enumerator within an enumeration
/// shall have an explicit value specified. Implicit values rely on
/// compiler-defined behavior and can lead to unexpected results if the enum
/// is modified.
///
/// Example of non-compliant code:
/// \code
///   enum Color {
///     RED,      // Implicit value 0
///     GREEN,    // Implicit value 1
///     BLUE      // Implicit value 2
///   };
/// \endcode
///
/// Example of compliant code:
/// \code
///   enum Color {
///     RED = 0,
///     GREEN = 1,
///     BLUE = 2
///   };
/// \endcode
class ExplicitEnumeratorValuesCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  ExplicitEnumeratorValuesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for enum declarations.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched enumerations and emits diagnostics.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXPLICITENUMERATORVALUESCHECK_H

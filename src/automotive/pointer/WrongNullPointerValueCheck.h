//===--- WrongNullPointerValueCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGNULLPOINTERVALUECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGNULLPOINTERVALUECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of integer zero instead of NULL or nullptr for null pointers.
///
/// Using the integer constant 0 to represent a null pointer is less clear
/// than using NULL (in C) or nullptr (in C++). Explicit null pointer constants
/// improve code readability and help distinguish pointer contexts from integer
/// contexts, reducing the risk of confusion and errors.
///
/// Related MISRA C:2025 Rule: 11.9 - The null pointer constant should be
/// defined as NULL rather than 0.
///
/// Example:
/// \code
///   int *ptr = 0;  // Warning: use NULL instead of 0
///   if (ptr == 0)  // Warning: use NULL for comparison
///     return;
/// \endcode
class WrongNullPointerValueCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  WrongNullPointerValueCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for null pointer value usage.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched null pointer values and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGNULLPOINTERVALUECHECK_H

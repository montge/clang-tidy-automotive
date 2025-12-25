//===--- AvoidUnrelatedPointerSubtractionCheck.h - clang-tidy --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNRELATEDPOINTERSUBTRACTIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNRELATEDPOINTERSUBTRACTIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-expressions
/// @brief Detects pointer subtraction between unrelated pointers.
///
/// This check detects pattern-based violations of pointer subtraction
/// where the pointers obviously point to different objects:
/// - Subtraction of pointers from different address-of expressions: &a - &b
/// - Subtraction of pointers from different 'new' allocations
///
/// Pointer subtraction is only valid when both pointers point to elements
/// of the same array (or one past the end). Subtracting unrelated pointers
/// leads to undefined behavior.
///
/// @par MISRA C++:2023 Rule 12.2 (Partial)
/// Subtraction between two pointers shall only be performed when they are
/// pointing to elements of the same array.
/// @par Category: Required
///
/// Example:
/// @code
///   int a, b;
///   int* p1 = &a;
///   int* p2 = &b;
///   ptrdiff_t diff = p1 - p2;  // Warning: unrelated pointers
///
///   int arr[10];
///   int* p3 = &arr[0];
///   int* p4 = &arr[5];
///   ptrdiff_t valid = p4 - p3; // OK: same array
/// @endcode
class AvoidUnrelatedPointerSubtractionCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidUnrelatedPointerSubtractionCheck(StringRef Name,
                                        ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidUnrelatedPointerSubtractionCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for pointer subtraction operations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched pointer subtraction operations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Checks if the expression is a pointer to a distinct object.
  /// Returns true if it's an address-of expression or new expression.
  bool isDistinctPointerOrigin(const Expr *E) const;

  /// Gets the canonical origin expression, stripping casts and parens.
  const Expr *getPointerOrigin(const Expr *E) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNRELATEDPOINTERSUBTRACTIONCHECK_H

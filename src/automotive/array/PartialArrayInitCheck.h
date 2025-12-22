//===--- PartialArrayInitCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_PARTIALARRAYINITCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_PARTIALARRAYINITCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-arrays
/// @brief Detects partial array initialization.
///
/// MISRA C:2025 Rule 9.7 requires that if an array is partially initialized,
/// all elements should be explicitly initialized. This prevents unintentional
/// reliance on zero-initialization of unspecified elements.
///
/// @par MISRA C:2025 Rule 9.7
/// If an array is partially initialized, all elements shall be explicitly
/// initialized.
/// @par Category: Advisory
///
/// Example:
/// @code
///   int arr[5] = {1, 2};  // Warning: only 2 of 5 elements initialized
///   int ok1[5] = {1, 2, 3, 4, 5};  // OK: all elements initialized
///   int ok2[5] = {};  // OK: explicit zero-init
///   char str[10] = "test";  // OK: string literal initialization
/// @endcode
class PartialArrayInitCheck : public ClangTidyCheck {
public:
  PartialArrayInitCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~PartialArrayInitCheck() override = default;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_PARTIALARRAYINITCHECK_H

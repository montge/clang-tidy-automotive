//===--- AvoidSizeofArrayParameterCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIZEOFARRAYPARAMETERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIZEOFARRAYPARAMETERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects sizeof applied to function parameters declared as arrays.
///
/// When a function parameter is declared with array syntax (e.g., int arr[10]),
/// it actually decays to a pointer. Using sizeof on such a parameter gives the
/// size of the pointer, not the array, which is almost always a bug.
///
/// @par MISRA C:2025 Rule 12.5 (Required)
/// The sizeof operator shall not have an operand which is a function parameter
/// declared as "array of type".
///
/// Example:
/// @code
///   void foo(int arr[10]) {
///     size_t s = sizeof(arr);  // Warning: gives sizeof(int*), not
///     sizeof(int[10])
///   }
///
///   void bar(int *ptr) {
///     size_t s = sizeof(ptr);  // Compliant: explicitly a pointer
///   }
/// @endcode
class AvoidSizeofArrayParameterCheck : public ClangTidyCheck {
public:
  AvoidSizeofArrayParameterCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIZEOFARRAYPARAMETERCHECK_H

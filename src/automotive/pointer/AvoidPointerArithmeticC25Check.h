//===--- AvoidPointerArithmeticC25Check.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICC25CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICC25CHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-pointer
/// @brief Detects pointer arithmetic using +, -, +=, -= operators.
///
/// The +, -, += and -= operators shall not be applied to an expression of
/// pointer type. Pointer arithmetic can lead to undefined behavior when the
/// result points outside the bounds of an array. Array subscript notation
/// should be used instead.
///
/// @par MISRA C:2025 Rule 18.4
/// The +, -, += and -= operators shall not be applied to an expression of
/// pointer type.
/// @par Category: Advisory
///
/// Example:
/// @code
///   int arr[10];
///   int *p = arr;
///   p = p + 1;        // Warning: pointer arithmetic with +
///   p += 2;           // Warning: compound assignment on pointer
///   int *q = p - 1;   // Warning: pointer arithmetic with -
///   ptrdiff_t d = q - p;  // Warning: pointer difference
///
///   // Compliant alternative using array indexing:
///   int value = arr[3];
/// @endcode
class AvoidPointerArithmeticC25Check : public ClangTidyCheck {
public:
  AvoidPointerArithmeticC25Check(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidPointerArithmeticC25Check() override = default;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICC25CHECK_H

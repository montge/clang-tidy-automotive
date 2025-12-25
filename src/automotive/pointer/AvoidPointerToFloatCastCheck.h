//===--- AvoidPointerToFloatCastCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTOFLOATCASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTOFLOATCASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-pointer
/// @brief Detects casts between pointer to object and non-integer arithmetic
/// types.
///
/// A cast shall not be performed between pointer to object and a non-integer
/// arithmetic type. Such conversions are undefined and have no meaningful
/// interpretation in any implementation.
///
/// @par MISRA C:2025 Rule 11.7
/// A cast shall not be performed between pointer to object and a non-integer
/// arithmetic type.
/// @par Category: Required
///
/// Example:
/// @code
///   int *p;
///   float f = (float)p;       // Warning: pointer to float
///   double d = (double)p;     // Warning: pointer to double
///   int *q = (int *)f;        // Warning: float to pointer
///   void *v = (void *)d;      // Warning: double to pointer
/// @endcode
class AvoidPointerToFloatCastCheck : public ClangTidyCheck {
public:
  AvoidPointerToFloatCastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidPointerToFloatCastCheck() override = default;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTOFLOATCASTCHECK_H

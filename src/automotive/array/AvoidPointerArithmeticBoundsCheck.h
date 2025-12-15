//===--- AvoidPointerArithmeticBoundsCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICBOUNDSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICBOUNDSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-array
/// @brief Detects potentially out-of-bounds pointer arithmetic.
///
/// Implements MISRA Rules 18.1, 18.2, 18.3, 18.5, 18.6:
/// - 18.1: Pointer arithmetic shall result in pointer to same array
/// - 18.2: Subtraction between pointers shall address same array
/// - 18.3: Relational operators shall not be applied to pointer types
///   except to address same array
/// - 18.5: Declaration shall contain no more than two levels of pointer
/// - 18.6: Address of auto object shall not persist after scope
///
/// @par Category: Required
///
/// Example:
/// @code
///   int a[10], b[10];
///   int *p = a + 20;       // Warning: may exceed bounds
///   int diff = p - b;      // Warning: different arrays
///   int ***ppp;            // Warning: more than 2 levels
/// @endcode
class AvoidPointerArithmeticBoundsCheck : public ClangTidyCheck {
public:
  AvoidPointerArithmeticBoundsCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidPointerArithmeticBoundsCheck() override = default;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  unsigned getPointerNestingLevel(QualType Type) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERARITHMETICBOUNDSCHECK_H

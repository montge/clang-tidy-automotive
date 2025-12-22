//===--- WellFormedForLoopCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WELLFORMEDFORLOOPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WELLFORMEDFORLOOPCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-statement
/// @brief Detects for loops that are not well-formed.
///
/// A well-formed for loop should have:
/// - A loop counter initialized in the init statement
/// - The loop counter used in the condition
/// - The loop counter modified only in the increment expression
/// - The loop counter not modified in the loop body
///
/// @par MISRA C:2025 Rule 14.2 (Required)
/// A for loop shall be well-formed.
///
/// Example:
/// @code
///   // Compliant:
///   for (int i = 0; i < 10; i++) { }
///
///   // Non-compliant - counter modified in body:
///   for (int i = 0; i < 10; i++) { i++; }
///
///   // Non-compliant - missing increment:
///   for (int i = 0; i < 10; ) { i++; }
/// @endcode
class WellFormedForLoopCheck : public ClangTidyCheck {
public:
  WellFormedForLoopCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WELLFORMEDFORLOOPCHECK_H

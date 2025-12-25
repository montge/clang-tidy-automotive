//===--- AvoidForLoopVarModificationCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFORLOOPVARMODIFICATIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFORLOOPVARMODIFICATIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-statement
/// @brief Detects modifications to for-loop iteration variables within the loop
/// body.
///
/// A for-loop iteration variable should not be modified in the body of the
/// loop. This prevents confusing control flow where the loop variable is
/// changed both by the increment expression and in the body, making the code
/// difficult to understand and maintain.
///
/// @par MISRA C++:2023 Rule 9.5.1 (Required)
/// A for-loop iteration variable should not be modified in the body of the
/// loop.
///
/// Example:
/// @code
///   // Non-compliant - loop variable modified in body:
///   for (int i = 0; i < 10; i++) {
///     i++;  // Warning: loop variable modified in body
///   }
///
///   for (int j = 0; j < n; j++) {
///     j = j + 2;  // Warning: loop variable modified in body
///   }
///
///   // Compliant - modification only in increment expression:
///   for (int i = 0; i < 10; i += 2) {
///     process(i);
///   }
/// @endcode
class AvoidForLoopVarModificationCheck : public ClangTidyCheck {
public:
  AvoidForLoopVarModificationCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFORLOOPVARMODIFICATIONCHECK_H

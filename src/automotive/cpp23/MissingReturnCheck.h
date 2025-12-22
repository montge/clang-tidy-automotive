//===--- MissingReturnCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGRETURNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGRETURNCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-function
/// @brief Detects non-void functions that may not return a value on all paths.
///
/// Non-void functions must explicitly return a value on all control flow paths.
/// Functions that fall off the end without a return statement violate this
/// rule.
///
/// @par MISRA C++:2023 Rule 9.6 (Required)
/// A function with non-void return type shall return a value on all paths.
///
/// Example:
/// @code
///   // Compliant:
///   int getValue(bool flag) {
///       if (flag) return 1;
///       return 0;
///   }
///
///   // Non-compliant - missing return on one path:
///   int getValue(bool flag) {
///       if (flag) return 1;
///       // Missing return here
///   }
/// @endcode
class MissingReturnCheck : public ClangTidyCheck {
public:
  MissingReturnCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGRETURNCHECK_H

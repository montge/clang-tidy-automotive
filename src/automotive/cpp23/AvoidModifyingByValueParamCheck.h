//===--- AvoidModifyingByValueParamCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMODIFYINGBYVALUEPARAMCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMODIFYINGBYVALUEPARAMCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-function
/// @brief Detects modifications to function parameters passed by value.
///
/// Parameters passed by value are copies, so modifications to them do not
/// affect the caller. This can be confusing and may indicate a bug where
/// the programmer intended to pass by reference.
///
/// @par MISRA C++:2023 Rule 13.3 (Required)
/// A parameter passed by value shall not be modified.
///
/// Example:
/// @code
///   void foo(int x) {
///     x = 10;        // Warning: modifying by-value parameter
///     x++;           // Warning: modifying by-value parameter
///   }
///
///   void bar(int& x) {
///     x = 10;        // Compliant: parameter is by reference
///   }
///
///   void baz(int x) {
///     int y = x + 1; // Compliant: using but not modifying
///   }
/// @endcode
class AvoidModifyingByValueParamCheck : public ClangTidyCheck {
public:
  AvoidModifyingByValueParamCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMODIFYINGBYVALUEPARAMCHECK_H

//===--- UseNullptrCheck.h - clang-tidy -------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USENULLPTRCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USENULLPTRCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Enforces the use of nullptr as the only null pointer constant.
///
/// MISRA C++:2023 Rule 7.11.2: nullptr shall be the only null pointer constant.
///
/// This check detects:
/// - Use of NULL macro in C++ code
/// - Use of integer literal 0 as a null pointer
/// - Implicit conversions of 0 to pointer types in all contexts
///
/// Example:
/// @code
///   // Compliant:
///   int* p = nullptr;
///   if (p == nullptr) {}
///
///   // Non-compliant:
///   int* p = 0;        // violation
///   int* p = NULL;     // violation
///   if (p == 0) {}     // violation
/// @endcode
class UseNullptrCheck : public ClangTidyCheck {
public:
  UseNullptrCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11; // nullptr requires C++11
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USENULLPTRCHECK_H

//===--- UninitializedAutoVarCheck.h - clang-tidy ----------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNINITIALIZEDAUTOVARCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNINITIALIZEDAUTOVARCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-declaration
/// @brief Detects local variables not initialized at declaration.
///
/// Variables with automatic storage duration (local variables) should be
/// initialized at the point of declaration to avoid use of uninitialized
/// values.
///
/// @par MISRA C++:2023 Rule 9.2 (Required)
/// Initialization of an automatic storage duration variable shall happen
/// at its declaration.
///
/// Example:
/// @code
///   // Compliant:
///   int x = 0;
///   std::string s = "hello";
///   auto value = getValue();
///
///   // Non-compliant - uninitialized local variables:
///   int x;          // Should be: int x = 0;
///   double d;       // Should be: double d = 0.0;
///   MyClass obj;    // OK if has default constructor
/// @endcode
class UninitializedAutoVarCheck : public ClangTidyCheck {
public:
  UninitializedAutoVarCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNINITIALIZEDAUTOVARCHECK_H

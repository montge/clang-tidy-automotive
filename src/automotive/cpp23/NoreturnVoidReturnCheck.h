//===--- NoreturnVoidReturnCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_NORETURNVOIDRETURNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_NORETURNVOIDRETURNCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-function
/// @brief Detects noreturn functions with non-void return types.
///
/// A function declared with [[noreturn]] or _Noreturn should have a void
/// return type since it never returns a value to its caller.
///
/// @par MISRA C++:2023 Rule 8.20
/// A noreturn function should have void return type.
/// @par Category: Advisory
///
/// Example:
/// @code
///   [[noreturn]] void terminate() {
///     std::exit(1);
///   }  // Compliant - void return type
///
///   [[noreturn]] int badTerminate() {
///     std::exit(1);
///   }  // Warning: non-void return type
/// @endcode
class NoreturnVoidReturnCheck : public ClangTidyCheck {
public:
  NoreturnVoidReturnCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~NoreturnVoidReturnCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11; // [[noreturn]] requires C++11
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_NORETURNVOIDRETURNCHECK_H

//===--- AvoidNoreturnReturnCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNORETURNRETURNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNORETURNRETURNCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-function
/// @brief Detects return statements in functions declared with noreturn
/// attribute.
///
/// A function declared with [[noreturn]] or _Noreturn shall not contain
/// a return statement, as this violates the semantic contract that the
/// function will never return to its caller.
///
/// @par MISRA C++:2023 Rule 8.19
/// A noreturn function shall not return.
/// @par Category: Advisory
///
/// Example:
/// @code
///   [[noreturn]] void terminate() {
///     std::exit(1);
///   }  // Compliant
///
///   [[noreturn]] void badTerminate() {
///     return;  // Warning: return in noreturn function
///   }
/// @endcode
class AvoidNoreturnReturnCheck : public ClangTidyCheck {
public:
  AvoidNoreturnReturnCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidNoreturnReturnCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11; // [[noreturn]] requires C++11
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNORETURNRETURNCHECK_H

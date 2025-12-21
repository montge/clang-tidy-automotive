//===--- AvoidExceptionCheck.h - clang-tidy ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDEXCEPTIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDEXCEPTIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects exception handling constructs in C++ code.
///
/// Exception handling (try, catch, throw) shall not be used in safety-critical
/// automotive software due to unpredictable timing and resource usage.
///
/// Related MISRA C++:2023 Rule 18.1
///
/// Example:
/// \code
///   try { ... }           // Warning: exception handling shall not be used
///   catch (...) { ... }   // Warning
///   throw std::exception(); // Warning
/// \endcode
class AvoidExceptionCheck : public ClangTidyCheck {
public:
  AvoidExceptionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDEXCEPTIONCHECK_H

//===--- AvoidDynamicMemoryCppCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDDYNAMICMEMORYCPPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDDYNAMICMEMORYCPPCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects dynamic memory allocation in C++ code.
///
/// Dynamic memory allocation (new, delete) shall not be used in safety-critical
/// automotive software due to potential memory fragmentation, allocation
/// failures, and unpredictable timing.
///
/// Related MISRA C++:2023 Rule 21.6
///
/// Example:
/// \code
///   int *p = new int;     // Warning: dynamic memory shall not be used
///   delete p;             // Warning
///   int *arr = new int[10]; // Warning
/// \endcode
class AvoidDynamicMemoryCppCheck : public ClangTidyCheck {
public:
  AvoidDynamicMemoryCppCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDDYNAMICMEMORYCPPCHECK_H

//===--- AvoidNonCompoundBodyCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONCOMPOUNDBODYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONCOMPOUNDBODYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects control statements with non-compound statement bodies in C++.
///
/// The body of an if, else, while, do, for, or range-based for statement
/// shall be a compound statement (enclosed in braces).
///
/// Related MISRA C++:2023 Rule 9.3
///
/// Example:
/// \code
///   if (x)
///     foo();    // Warning: not a compound statement
///   if (x) {
///     foo();    // OK
///   }
/// \endcode
class AvoidNonCompoundBodyCheck : public ClangTidyCheck {
public:
  AvoidNonCompoundBodyCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONCOMPOUNDBODYCHECK_H

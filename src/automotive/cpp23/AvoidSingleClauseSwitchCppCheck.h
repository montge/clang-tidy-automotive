//===--- AvoidSingleClauseSwitchCppCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSINGLECLAUSESWITCHCPPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSINGLECLAUSESWITCHCPPCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects switch statements with fewer than two non-empty switch clauses.
///
/// A switch statement shall have at least two non-empty switch clauses.
/// A switch with only one clause should be replaced with an if statement.
///
/// Related MISRA C++:2023 Rule 9.4
///
/// Example:
/// \code
///   switch (x) {
///     case 1: break;  // Warning: only one clause
///   }
/// \endcode
class AvoidSingleClauseSwitchCppCheck : public ClangTidyCheck {
public:
  AvoidSingleClauseSwitchCppCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSINGLECLAUSESWITCHCPPCHECK_H

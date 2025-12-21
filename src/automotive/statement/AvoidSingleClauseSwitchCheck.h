//===--- AvoidSingleClauseSwitchCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSINGLECLAUSESWITCHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSINGLECLAUSESWITCHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects switch statements with fewer than two switch-clauses.
///
/// Every switch statement shall have at least two switch-clauses to be
/// meaningful. A switch with only one case should be replaced with an if.
///
/// Related MISRA C:2025 Rule 16.6
///
/// Example:
/// \code
///   switch (x) {
///     case 1: break;  // Warning: only one clause
///   }
/// \endcode
class AvoidSingleClauseSwitchCheck : public ClangTidyCheck {
public:
  AvoidSingleClauseSwitchCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSINGLECLAUSESWITCHCHECK_H

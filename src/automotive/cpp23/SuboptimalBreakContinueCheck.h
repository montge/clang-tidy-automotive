//===--- SuboptimalBreakContinueCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_SUBOPTIMALBREAKCONTINUECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_SUBOPTIMALBREAKCONTINUECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects suboptimal placement of break and continue statements.
///
/// MISRA C++:2023 Rule 6.8: A break or continue statement shall be placed
/// at the end of a compound statement to improve readability.
///
/// This check flags break/continue statements that are followed by other
/// statements in the same compound statement. Such statements should be
/// moved to the end or restructured for better code clarity.
///
/// Example:
/// \code
///   while (condition) {
///     if (x > 0)
///       break;
///     foo();      // Warning: code after break (unreachable)
///   }
///
///   // Better:
///   while (condition) {
///     if (x > 0)
///       break;
///   }
/// \endcode
class SuboptimalBreakContinueCheck : public ClangTidyCheck {
public:
  SuboptimalBreakContinueCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_SUBOPTIMALBREAKCONTINUECHECK_H

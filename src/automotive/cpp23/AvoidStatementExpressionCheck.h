//===--- AvoidStatementExpressionCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTATEMENTEXPRESSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTATEMENTEXPRESSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects GNU statement expression extension (({ ... })).
///
/// Statement expressions are a GNU extension that allows compound statements
/// to be used as expressions. This extension should not be used in portable
/// C++ code as it is not part of the standard and reduces code portability.
///
/// Related MISRA C++:2023 Rule 8.18
///
/// Example:
/// \code
///   int x = ({ int y = 5; y * 2; });  // Warning: statement expression
/// \endcode
class AvoidStatementExpressionCheck : public ClangTidyCheck {
public:
  AvoidStatementExpressionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTATEMENTEXPRESSIONCHECK_H

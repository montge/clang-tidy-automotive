//===--- AvoidAssignmentInSubExpressionCheck.h - clang-tidy ------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASSIGNMENTINSUBEXPRESSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASSIGNMENTINSUBEXPRESSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-expression
/// @brief Detects assignment operators used as sub-expressions.
///
/// Assignment operators should only be used as expression statements,
/// not as part of larger expressions such as conditions.
///
/// @par MISRA C++:2023 Rule 6.4 (Required)
/// The result of an assignment operator shall not be used.
///
/// Example:
/// @code
///   // Compliant:
///   x = getValue();
///   if (x != 0) { }
///
///   // Non-compliant - assignment in condition:
///   if (x = getValue()) { }
///
///   // Non-compliant - assignment in larger expression:
///   y = (x = 5) + 1;
/// @endcode
class AvoidAssignmentInSubExpressionCheck : public ClangTidyCheck {
public:
  AvoidAssignmentInSubExpressionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASSIGNMENTINSUBEXPRESSIONCHECK_H

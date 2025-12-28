//===--- BooleanControlExpressionCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_BOOLEANCONTROLEXPRESSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_BOOLEANCONTROLEXPRESSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-control-flow
/// @brief Ensures control expressions are of Boolean type.
///
/// Control expressions in if, while, for, and do-while statements should
/// evaluate to Boolean type to avoid confusion and potential bugs from
/// implicit conversions.
///
/// @par MISRA C++:2023 Rule 16.5
/// A control expression shall be of essentially Boolean type.
/// @par Category: Required
///
/// Example:
/// @code
///   // Non-compliant:
///   int x = 5;
///   if (x) { }           // Warning - integer used as Boolean
///   while (ptr) { }      // Warning - pointer used as Boolean
///
///   // Compliant:
///   if (x != 0) { }      // OK - explicit comparison
///   if (x == 5) { }      // OK - Boolean result
///   bool flag = true;
///   if (flag) { }        // OK - Boolean type
/// @endcode
class BooleanControlExpressionCheck : public ClangTidyCheck {
public:
  BooleanControlExpressionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~BooleanControlExpressionCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isEssentiallyBoolean(const Expr *E, ASTContext *Context) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_BOOLEANCONTROLEXPRESSIONCHECK_H

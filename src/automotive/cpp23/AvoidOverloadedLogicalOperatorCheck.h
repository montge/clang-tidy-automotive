//===--- AvoidOverloadedLogicalOperatorCheck.h - clang-tidy -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOVERLOADEDLOGICALOPERATORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOVERLOADEDLOGICALOPERATORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects overloaded operator&& and operator||.
///
/// MISRA C++:2023 Rule 16.5: A class should only define an overloaded
/// operator && or operator || if it does not rely on short-circuit evaluation.
///
/// When these operators are overloaded, short-circuit evaluation is lost,
/// which can lead to unexpected behavior.
class AvoidOverloadedLogicalOperatorCheck : public ClangTidyCheck {
public:
  AvoidOverloadedLogicalOperatorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOVERLOADEDLOGICALOPERATORCHECK_H

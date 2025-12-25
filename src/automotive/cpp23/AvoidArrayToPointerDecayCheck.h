//===--- AvoidArrayToPointerDecayCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDARRAYTOPOINTERDECAYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDARRAYTOPOINTERDECAYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Checks that array identifiers are not used in expressions with pointer
/// semantics where the array type is lost.
///
/// MISRA C++:2023 Rule 5.10 (Required)
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/automotive/cpp23-req-5.10.html
class AvoidArrayToPointerDecayCheck : public ClangTidyCheck {
public:
  AvoidArrayToPointerDecayCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDARRAYTOPOINTERDECAYCHECK_H

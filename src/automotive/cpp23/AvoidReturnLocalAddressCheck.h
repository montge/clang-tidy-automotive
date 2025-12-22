//===--- AvoidReturnLocalAddressCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDRETURNLOCALADDRESSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDRETURNLOCALADDRESSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects functions that return pointers or references to local variables.
///
/// MISRA C++:2023 Rule 6.8.2 (Mandatory): A function must not return a
/// reference or a pointer to a local variable with automatic storage duration.
///
/// This includes:
/// - Returning address of local variable (&local)
/// - Returning reference to local variable
/// - Function parameters passed by value (returning &param or param reference)
/// - Lambdas capturing locals by reference
class AvoidReturnLocalAddressCheck : public ClangTidyCheck {
public:
  AvoidReturnLocalAddressCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDRETURNLOCALADDRESSCHECK_H

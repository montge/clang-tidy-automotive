//===--- AvoidLambdaThisCaptureCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDLAMBDATHISCAPTURECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDLAMBDATHISCAPTURECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects lambda expressions that implicitly capture 'this'.
///
/// MISRA C++:2023 Rule 8.1.1 (Required): A lambda shall not implicitly capture
/// 'this'. Lambdas within member functions should use explicit [this] or
/// [*this] (C++17) capture instead of relying on [=] or [&] to capture 'this'
/// implicitly. This makes the code more explicit and prevents unintended
/// captures.
///
/// Example:
/// @code
///   class MyClass {
///     int value = 42;
///     void foo() {
///       auto f1 = [=]() { return value; };  // Warning: implicit 'this'
///       capture auto f2 = [&]() { return value; };  // Warning: implicit
///       'this' capture auto f3 = [this]() { return value; }; // Compliant:
///       explicit 'this' auto f4 = [*this]() { return value; }; // Compliant:
///       explicit copy
///     }
///   };
/// @endcode
class AvoidLambdaThisCaptureCheck : public ClangTidyCheck {
public:
  AvoidLambdaThisCaptureCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDLAMBDATHISCAPTURECHECK_H

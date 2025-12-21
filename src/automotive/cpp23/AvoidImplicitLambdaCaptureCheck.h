//===--- AvoidImplicitLambdaCaptureCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIMPLICITLAMBDACAPTURECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIMPLICITLAMBDACAPTURECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-lambda
/// @brief Detects lambdas that implicitly capture variables.
///
/// Implicit captures using [=] or [&] can lead to unintended variable
/// captures, dangling references, or unclear code. This check requires all
/// lambda captures to be explicitly listed.
///
/// @par MISRA C++:2023 Rule 0.1 (Required)
/// A variable shall not be implicitly captured in a lambda.
///
/// Example:
/// @code
///   int x = 1, y = 2;
///   auto f1 = [=]() { return x + y; };    // Warning: implicit capture
///   auto f2 = [&]() { return x + y; };    // Warning: implicit capture
///   auto f3 = [x, y]() { return x + y; }; // Compliant: explicit capture
///   auto f4 = [&x, &y]() { return x + y; }; // Compliant: explicit capture
/// @endcode
class AvoidImplicitLambdaCaptureCheck : public ClangTidyCheck {
public:
  AvoidImplicitLambdaCaptureCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIMPLICITLAMBDACAPTURECHECK_H

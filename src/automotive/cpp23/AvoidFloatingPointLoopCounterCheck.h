//===--- AvoidFloatingPointLoopCounterCheck.h - clang-tidy --------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLOATINGPOINTLOOPCOUNTERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLOATINGPOINTLOOPCOUNTERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-statement
/// @brief Detects floating-point types used as loop counters.
///
/// Floating-point types should not be used as loop counters because
/// accumulated rounding errors can cause unexpected loop behavior.
///
/// @par MISRA C++:2023 Rule 6.5 (Required)
/// A for-loop counter shall not have floating-point type.
///
/// Example:
/// @code
///   // Compliant:
///   for (int i = 0; i < 10; i++) { }
///
///   // Non-compliant - floating-point loop counter:
///   for (float f = 0.0f; f < 10.0f; f += 0.1f) { }
///
///   // Non-compliant - double loop counter:
///   for (double d = 0.0; d < 1.0; d += 0.01) { }
/// @endcode
class AvoidFloatingPointLoopCounterCheck : public ClangTidyCheck {
public:
  AvoidFloatingPointLoopCounterCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLOATINGPOINTLOOPCOUNTERCHECK_H

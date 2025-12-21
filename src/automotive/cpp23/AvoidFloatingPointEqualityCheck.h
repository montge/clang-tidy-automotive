//===--- AvoidFloatingPointEqualityCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLOATINGPOINTEQUALITYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLOATINGPOINTEQUALITYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-expression
/// @brief Detects equality/inequality comparisons on floating-point types.
///
/// Floating-point numbers have inherent precision issues, so direct equality
/// comparisons (== or !=) can produce unexpected results. This check warns
/// when floating-point expressions are tested for equality or inequality.
///
/// @par MISRA C++:2023 Rule 6.7 (Required)
/// Floating-point expressions shall not be tested for equality or inequality.
///
/// Example:
/// @code
///   float a = 0.1f + 0.2f;
///   if (a == 0.3f) { }  // Warning: floating-point equality comparison
///   if (a != 0.3f) { }  // Warning: floating-point inequality comparison
///
///   // Compliant alternatives:
///   if (std::abs(a - 0.3f) < epsilon) { }  // Use epsilon comparison
/// @endcode
class AvoidFloatingPointEqualityCheck : public ClangTidyCheck {
public:
  AvoidFloatingPointEqualityCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDFLOATINGPOINTEQUALITYCHECK_H

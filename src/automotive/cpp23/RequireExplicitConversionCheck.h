//===--- RequireExplicitConversionCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_REQUIREEXPLICITCONVERSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_REQUIREEXPLICITCONVERSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-classes
/// @brief Detects non-explicit conversion operators.
///
/// User-defined conversion operators should be marked explicit to prevent
/// unintended implicit conversions. Implicit conversions can lead to
/// unexpected behavior, subtle bugs, and make code harder to understand and
/// maintain.
///
/// @par MISRA C++:2023 Rule 10.4.1
/// A conversion function shall be explicit.
/// @par Category: Advisory
///
/// Example:
/// @code
///   class Wrapper {
///     int value;
///   public:
///     // Non-compliant: implicit conversion operator
///     operator int() const { return value; }
///   };
///
///   class SafeWrapper {
///     int value;
///   public:
///     // Compliant: explicit conversion operator
///     explicit operator int() const { return value; }
///   };
/// @endcode
class RequireExplicitConversionCheck : public ClangTidyCheck {
public:
  RequireExplicitConversionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~RequireExplicitConversionCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11; // explicit conversion operators require C++11
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_REQUIREEXPLICITCONVERSIONCHECK_H

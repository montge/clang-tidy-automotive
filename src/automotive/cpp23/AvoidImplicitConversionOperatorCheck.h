//===--- AvoidImplicitConversionOperatorCheck.h - clang-tidy ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITCONVERSIONOPERATORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITCONVERSIONOPERATORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-classes
/// @brief Detects non-explicit conversion operators.
///
/// Implicit conversion operators can lead to unexpected type conversions and
/// make code harder to understand. Conversion operators should be marked
/// explicit to require an explicit cast.
///
/// @par MISRA C++:2023 Rule 15.1.2
/// Conversion operators shall be explicit.
/// @par Category: Required
///
/// Example:
/// @code
///   class Wrapper {
///     int value;
///   public:
///     operator int() { return value; }  // Warning: should be explicit
///   };
/// @endcode
class AvoidImplicitConversionOperatorCheck : public ClangTidyCheck {
public:
  AvoidImplicitConversionOperatorCheck(StringRef Name,
                                       ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidImplicitConversionOperatorCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11; // explicit conversion operators require C++11
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITCONVERSIONOPERATORCHECK_H

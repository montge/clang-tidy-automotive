//===--- AvoidImplicitConversionCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITCONVERSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITCONVERSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-expressions
/// @brief Detects implicit conversions that should be explicit.
///
/// Implicit conversions can lead to unexpected behavior, especially when
/// converting between different numeric types or when precision loss occurs.
/// This check identifies implicit conversions in function arguments and
/// return statements that should be made explicit.
///
/// @par MISRA C++:2023 Rule 8.3.1
/// Implicit conversions shall be made explicit.
/// @par Category: Required
///
/// Example:
/// @code
///   void foo(double d);
///   int x = 42;
///   foo(x);  // Warning: implicit conversion from int to double
///   foo(static_cast<double>(x));  // OK: explicit conversion
/// @endcode
class AvoidImplicitConversionCheck : public ClangTidyCheck {
public:
  AvoidImplicitConversionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidImplicitConversionCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isSignificantConversion(QualType From, QualType To,
                               const ASTContext &Ctx) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITCONVERSIONCHECK_H

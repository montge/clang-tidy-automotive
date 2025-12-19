//===--- AvoidNarrowingConversionCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNARROWINGCONVERSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNARROWINGCONVERSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-expressions
/// @brief Detects narrowing conversions that may lose data.
///
/// Narrowing conversions can silently lose information and are a common source
/// of bugs. This check detects implicit conversions that could result in data
/// loss, such as converting a larger integer type to a smaller one, or
/// converting floating-point to integer.
///
/// @par MISRA C++:2023 Rule 8.4.1
/// Narrowing conversions shall not be used.
/// @par Category: Required
///
/// Example:
/// @code
///   double d = 3.14;
///   int x = d;  // Warning: narrowing conversion from double to int
/// @endcode
class AvoidNarrowingConversionCheck : public ClangTidyCheck {
public:
  AvoidNarrowingConversionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidNarrowingConversionCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isNarrowingConversion(QualType FromType, QualType ToType,
                             const ASTContext &Ctx) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNARROWINGCONVERSIONCHECK_H

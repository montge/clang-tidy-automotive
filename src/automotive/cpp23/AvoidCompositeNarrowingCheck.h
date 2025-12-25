//===--- AvoidCompositeNarrowingCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCOMPOSITNARROWINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCOMPOSITNARROWINGCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-expression
/// @brief Detects when composite expressions are assigned to narrower essential
/// types.
///
/// This check enforces MISRA C++:2023 Rule 10.4, which requires that the value
/// of a composite expression (result of arithmetic operations) shall not be
/// assigned to a narrower essential type. This prevents loss of data from
/// arithmetic operations due to implicit narrowing conversions.
///
/// @par MISRA C++:2023 Rule 10.4 (Required)
/// The value of a composite expression shall not be assigned to a narrower
/// essential type.
///
/// Example:
/// @code
///   int32_t a = 1000;
///   int32_t b = 2000;
///   int16_t result = a + b;  // Warning: composite expression assigned to
///   narrower type
///
///   // Compliant alternative:
///   int32_t result = a + b;  // OK: same type
///   int16_t result2 = static_cast<int16_t>(a + b);  // OK: explicit cast
/// @endcode
class AvoidCompositeNarrowingCheck : public ClangTidyCheck {
public:
  AvoidCompositeNarrowingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isCompositeExpression(const Expr *E) const;
  bool isNarrowingAssignment(QualType ExprType, QualType DestType,
                             const ASTContext &Ctx) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCOMPOSITNARROWINGCHECK_H

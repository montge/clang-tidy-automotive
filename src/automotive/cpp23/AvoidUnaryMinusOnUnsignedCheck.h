//===--- AvoidUnaryMinusOnUnsignedCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNARYMINUSONUNSIGNEDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNARYMINUSONUNSIGNEDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-operator
/// @brief Detects unary minus operator applied to unsigned integer types.
///
/// Applying the unary minus operator to an unsigned type causes wrap-around
/// to a large positive value, which is often unintentional and can lead to
/// subtle bugs. The behavior is well-defined but rarely desired.
///
/// @par MISRA C++:2023 Rule 6.0.4 (Required)
/// The unary minus operator shall not be applied to an expression whose
/// underlying type is unsigned.
///
/// Example:
/// @code
///   unsigned int x = 5;
///   int y = -x;  // Warning: unary minus on unsigned type
///
///   uint32_t val = 10;
///   auto neg = -val;  // Warning: unary minus on unsigned type
///
///   // Compliant alternatives:
///   int x_signed = 5;
///   int y = -x_signed;  // OK: signed type
///
///   unsigned u = 5;
///   int y2 = -static_cast<int>(u);  // OK: explicit cast first
/// @endcode
class AvoidUnaryMinusOnUnsignedCheck : public ClangTidyCheck {
public:
  AvoidUnaryMinusOnUnsignedCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isUnsignedIntegerType(QualType Type) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNARYMINUSONUNSIGNEDCHECK_H

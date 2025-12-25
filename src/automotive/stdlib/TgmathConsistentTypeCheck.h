//===--- TgmathConsistentTypeCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TGMATHCONSISTENTTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TGMATHCONSISTENTTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Checks that multi-argument tgmath.h macros have consistent types.
///
/// All operand arguments to any multi-argument type-generic macros declared
/// in <tgmath.h> shall have the same standard type after integer promotion.
///
/// Affected functions: atan2, copysign, fdim, fma, fmax, fmin, fmod, frexp,
/// hypot, ldexp, nextafter, nexttoward, pow, remainder, remquo, scalbn, scalbln
///
/// Example:
/// @code
///   #include <tgmath.h>
///   float f = 1.0f;
///   double d = 2.0;
///   // Non-compliant: float and double mixed
///   double r = pow(f, d);
///   // Compliant: both double
///   double s = pow(d, d);
/// @endcode
class TgmathConsistentTypeCheck : public ClangTidyCheck {
public:
  TgmathConsistentTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TGMATHCONSISTENTTYPECHECK_H

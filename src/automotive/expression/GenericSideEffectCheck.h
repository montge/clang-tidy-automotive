//===--- GenericSideEffectCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICSIDEEFFECTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICSIDEEFFECTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Detects side effects in the controlling expression of _Generic.
///
/// A generic selection that is not expanded from a macro shall not contain
/// potential side effects in the controlling expression. The controlling
/// expression is never evaluated - only its type is used.
///
/// Example:
/// @code
///   int x;
///   // Non-compliant: ++x has side effect but won't be evaluated
///   _Generic(++x, int: 1, default: 0);
///
///   // Compliant: no side effects
///   _Generic(x, int: 1, default: 0);
/// @endcode
class GenericSideEffectCheck : public ClangTidyCheck {
public:
  GenericSideEffectCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICSIDEEFFECTCHECK_H

//===--- WrongConditionalOperandTypeCheck.h - clang-tidy --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGCONDITIONALOPERANDTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGCONDITIONALOPERANDTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects incompatible pointer and integer types as operands to the
/// conditional operator.
///
/// MISRA C:2025 Rule 7.5 (Mandatory):
/// Incompatible pointer and integer types shall not be used as operands
/// to the conditional operator.
///
/// Example:
/// \code
///   int *p;
///   int x = cond ? p : 0;     // Warning: pointer and integer mix
///   int *y = cond ? p : NULL; // OK: NULL is allowed
/// \endcode
class WrongConditionalOperandTypeCheck : public ClangTidyCheck {
public:
  WrongConditionalOperandTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGCONDITIONALOPERANDTYPECHECK_H

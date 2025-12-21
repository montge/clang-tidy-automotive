//===--- AvoidQualifiedFunctionTypeCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDQUALIFIEDFUNCTIONTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDQUALIFIEDFUNCTIONTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects type-qualified function types.
///
/// A function type shall not be const or volatile qualified. Such qualifiers
/// on function types have no meaning and may indicate programmer confusion.
///
/// Related MISRA C:2025 Rule 17.13
///
/// Example:
/// \code
///   typedef void func_t(void);
///   const func_t f;  // Warning: const-qualified function type
/// \endcode
class AvoidQualifiedFunctionTypeCheck : public ClangTidyCheck {
public:
  AvoidQualifiedFunctionTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDQUALIFIEDFUNCTIONTYPECHECK_H

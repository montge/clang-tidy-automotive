//===--- UnreachableCodeCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNREACHABLECODECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNREACHABLECODECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects unreachable code (MISRA C:2012 Rule 2.1).
///
/// Code after unconditional control flow statements (return, break, continue,
/// goto, throw) is unreachable and should not exist in the codebase.
///
/// \code
/// void func() {
///   return;
///   int x = 5;  // Unreachable - violation
/// }
/// \endcode
class UnreachableCodeCheck : public ClangTidyCheck {
public:
  UnreachableCodeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNREACHABLECODECHECK_H

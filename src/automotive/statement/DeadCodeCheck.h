//===--- DeadCodeCheck.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DEADCODECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DEADCODECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects dead code that has no effect (MISRA C:2025 Rule 2.2).
///
/// Dead code is code that serves no purpose and has no observable effect.
/// This includes:
/// - Statements with no side effects (e.g., expressions that don't modify state)
/// - Operations whose results are never used
/// - Assignments to variables that are never read
///
/// \code
/// void func() {
///   int x;
///   x + 5;      // Dead code - expression has no effect
///   x = 10;     // May be dead if x is never read after this
/// }
/// \endcode
class DeadCodeCheck : public ClangTidyCheck {
public:
  DeadCodeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check if an expression has side effects
  bool hasSideEffects(const Expr *E) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DEADCODECHECK_H

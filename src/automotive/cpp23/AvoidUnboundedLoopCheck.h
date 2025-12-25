//===--- AvoidUnboundedLoopCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNBOUNDEDLOOPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNBOUNDEDLOOPCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-statement
/// @brief Detects loops without clear termination conditions.
///
/// Loop counters should have well-defined bounds to ensure loops terminate
/// properly. This check detects:
/// - Loops with literal 'true' or constant conditions
/// - While/do-while loops without obvious termination logic
/// - Loops where condition variables are not clearly bounded
///
/// @par MISRA C++:2023 Rule 6.3 (Required)
/// Loop counters should have well-defined bounds.
///
/// Example:
/// @code
///   // Compliant:
///   for (int i = 0; i < 10; i++) { }
///   while (count < max) { count++; }
///
///   // Non-compliant - infinite loop:
///   while (true) { /* ... */ }
///
///   // Non-compliant - constant condition:
///   for (;;) { /* ... */ }
///
///   // Non-compliant - unbounded while:
///   while (1) { /* ... */ }
/// @endcode
class AvoidUnboundedLoopCheck : public ClangTidyCheck {
public:
  AvoidUnboundedLoopCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isConstantCondition(const Expr *E, ASTContext &Context) const;
  bool isTrueConstant(const Expr *E, ASTContext &Context) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNBOUNDEDLOOPCHECK_H

//===--- AvoidSideEffectInInitializerCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINITIALIZERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINITIALIZERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects side effects in initializer lists.
///
/// Implements MISRA Rules 13.1, 13.2, 13.3:
/// - 13.1: Initializer lists shall not contain persistent side effects
/// - 13.2: Value of expression and side effects shall be same under any
///   evaluation order
/// - 13.3: Full expression with increment/decrement shall have no other
///   side effects
///
/// @par Category: Required
///
/// Example:
/// @code
///   int a = 0;
///   int arr[] = {a++, a++};  // Warning: side effects in initializer
///   int x = a++ + a++;       // Warning: unsequenced side effects
/// @endcode
class AvoidSideEffectInInitializerCheck : public ClangTidyCheck {
public:
  AvoidSideEffectInInitializerCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidSideEffectInInitializerCheck() override = default;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool hasSideEffect(const Expr *E) const;
  unsigned countSideEffects(const Expr *E) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIDEEFFECTINITIALIZERCHECK_H

//===--- AvoidCompositeExpressionMismatchCheck.h - clang-tidy ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMPOSITEEXPRESSIONMISMATCHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMPOSITEEXPRESSIONMISMATCHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects composite expression type mismatches in assignments.
///
/// Implements MISRA Rules 10.6, 10.7, and 10.8:
/// - 10.6: Composite expression shall not be assigned to wider type
/// - 10.7: Composite expression shall not use operand of wider type
/// - 10.8: Composite expression shall not be cast to different category
///
/// @par MISRA C:2025 Rules 10.6, 10.7, 10.8
/// Composite expressions have an essential type that should be preserved.
/// @par Category: Required
///
/// Example:
/// @code
///   uint16_t a = 1000, b = 2000;
///   uint32_t result = a + b;  // Warning: composite assigned to wider
///   uint32_t c = (uint32_t)(a + b); // Warning: cast of composite
/// @endcode
class AvoidCompositeExpressionMismatchCheck : public ClangTidyCheck {
public:
  AvoidCompositeExpressionMismatchCheck(StringRef Name,
                                        ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  unsigned getTypeWidth(QualType Type, const ASTContext &Ctx) const;
  bool isCompositeExpression(const Expr *E) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCOMPOSITEEXPRESSIONMISMATCHCHECK_H

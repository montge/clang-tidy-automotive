//===--- AvoidBoolArithmeticCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBOOLARITHMETICCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBOOLARITHMETICCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-expression
/// @brief Detects improper use of bool expressions with built-in operators.
///
/// This check ensures that expressions with type bool are only used with
/// specific allowed operators: assignment (=), logical operators (&&, ||, !),
/// equality operators (==, !=), and the unary address-of operator (&).
///
/// @par MISRA C++:2023 Rule 6.2.1 (Required)
/// Expressions with type bool shall not be used as operands to built-in
/// operators other than the assignment operator =, the logical operators
/// &&, ||, !, the equality operators == and !=, and the unary & operator.
///
/// Example:
/// @code
///   bool b1 = true, b2 = false;
///
///   // Non-compliant: arithmetic operations
///   int x = b1 + b2;      // Warning
///   int y = b1 * 2;       // Warning
///   bool b3 = b1++;       // Warning
///
///   // Non-compliant: bitwise operations
///   int z = b1 | b2;      // Warning
///   int w = b1 << 1;      // Warning
///
///   // Non-compliant: relational comparisons
///   bool c = b1 < b2;     // Warning
///
///   // Compliant: allowed operators
///   bool result = b1 && b2;   // OK: logical AND
///   bool equal = b1 == b2;    // OK: equality
///   bool notB = !b1;          // OK: logical NOT
///   bool *ptr = &b1;          // OK: address-of
///   b1 = b2;                  // OK: assignment
/// @endcode
class AvoidBoolArithmeticCheck : public ClangTidyCheck {
public:
  AvoidBoolArithmeticCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkBinaryOperator(const BinaryOperator *BinOp, ASTContext *Context);
  void checkUnaryOperator(const UnaryOperator *UnaryOp, ASTContext *Context);
  bool isBoolType(QualType Type) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBOOLARITHMETICCHECK_H

//===--- AvoidSignedBitwiseCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIGNEDBITWISECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIGNEDBITWISECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-operator
/// @brief Detects bitwise operations on signed integer operands.
///
/// Bitwise operations on signed integers can lead to implementation-defined
/// or undefined behavior, particularly with right shift operations and sign
/// bit manipulation. This check warns when bitwise operators are used with
/// signed operands.
///
/// @par MISRA C++:2023 Rule 8.8.1 (Required)
/// Bitwise operations shall not be performed on signed operands.
///
/// Example:
/// @code
///   int x = -1;
///   int y = x << 1;   // Warning: left shift on signed operand
///   int z = x & 0xFF; // Warning: bitwise AND on signed operand
///   int w = ~x;       // Warning: bitwise NOT on signed operand
///
///   // Compliant alternatives:
///   unsigned int ux = static_cast<unsigned int>(x);
///   unsigned int uy = ux << 1;  // OK: unsigned operands
///   unsigned int uz = ux & 0xFF; // OK: unsigned operands
/// @endcode
class AvoidSignedBitwiseCheck : public ClangTidyCheck {
public:
  AvoidSignedBitwiseCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkBinaryOperator(const BinaryOperator *BinOp, ASTContext *Context);
  void checkUnaryOperator(const UnaryOperator *UnaryOp, ASTContext *Context);
  bool isSignedIntegerType(QualType Type) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIGNEDBITWISECHECK_H

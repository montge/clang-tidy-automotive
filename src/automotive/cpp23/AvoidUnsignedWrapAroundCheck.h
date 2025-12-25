//===--- AvoidUnsignedWrapAroundCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNSIGNEDWRAPAROUNDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNSIGNEDWRAPAROUNDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-arithmetic
/// @brief Detects unsigned integer operations that may cause wrap-around.
///
/// Unsigned integer operations wrap around modulo 2^N when the result exceeds
/// the maximum value or underflows below zero. While well-defined behavior,
/// wrap-around is often unintentional and can lead to serious bugs in safety-
/// critical code, particularly in subtraction operations and operations near
/// the type's maximum value.
///
/// @par MISRA C++:2023 Rule 7.2 (Required)
/// Unsigned integer operations shall not wrap around.
///
/// This check focuses on detecting potentially unsafe patterns:
/// - Subtraction where the result might underflow (right operand > left)
/// - Addition/multiplication near maximum values that might overflow
/// - Compound assignment operators that may cause wrap-around
///
/// Example:
/// @code
///   unsigned int a = 10;
///   unsigned int b = 20;
///   unsigned int c = a - b;  // Warning: potential underflow wrap-around
///
///   unsigned int x = UINT_MAX - 5;
///   unsigned int y = x + 10;  // Warning: potential overflow wrap-around
///
///   unsigned int i = 5;
///   i -= 10;  // Warning: compound assignment may cause wrap-around
///
///   // Compliant alternatives:
///   if (a >= b) {
///       unsigned int c = a - b;  // OK: checked before subtraction
///   }
///
///   // Or use signed arithmetic when negative values are possible
///   int d = static_cast<int>(a) - static_cast<int>(b);  // OK
/// @endcode
class AvoidUnsignedWrapAroundCheck : public ClangTidyCheck {
public:
  AvoidUnsignedWrapAroundCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkBinaryOperator(const BinaryOperator *BinOp, ASTContext *Context);
  void checkCompoundAssignment(const CompoundAssignOperator *CompoundOp,
                               ASTContext *Context);
  bool isUnsignedIntegerType(QualType Type) const;
  bool mayWrapAround(const BinaryOperator *BinOp, ASTContext *Context) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNSIGNEDWRAPAROUNDCHECK_H

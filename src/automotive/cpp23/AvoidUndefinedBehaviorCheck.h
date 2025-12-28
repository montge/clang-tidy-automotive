//===--- AvoidUndefinedBehaviorCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNDEFINEDBEHAVIORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNDEFINEDBEHAVIORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-undefined-behavior
/// @brief Detects statically-detectable undefined behavior.
///
/// Undefined behavior can lead to unpredictable program behavior and security
/// vulnerabilities. This check detects common patterns of statically-detectable
/// undefined behavior such as:
/// - Division by zero
/// - Shift by negative or excessive amounts
/// - Null pointer dereference (when statically detectable)
/// - Signed integer overflow in constant expressions
/// - Use of uninitialized variables (partial detection)
///
/// @par MISRA C++:2023 Rule 0.3
/// Statically-detectable undefined behavior shall not occur.
/// @par Category: Required
///
/// Example:
/// @code
///   int x = 10 / 0;  // Warning: division by zero
///   int y = 1 << 32; // Warning: shift amount >= width of type
/// @endcode
class AvoidUndefinedBehaviorCheck : public ClangTidyCheck {
public:
  AvoidUndefinedBehaviorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidUndefinedBehaviorCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkDivisionByZero(const BinaryOperator *BinOp, ASTContext *Context);
  void checkShiftAmount(const BinaryOperator *BinOp, ASTContext *Context);
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNDEFINEDBEHAVIORCHECK_H

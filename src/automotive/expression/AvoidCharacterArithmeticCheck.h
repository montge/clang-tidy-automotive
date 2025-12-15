//===--- AvoidCharacterArithmeticCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCHARACTERARITHMETICCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCHARACTERARITHMETICCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects inappropriate character type arithmetic operations.
///
/// Implements MISRA Rule 10.2: Expressions of essentially character type
/// shall not be used inappropriately in addition and subtraction operations.
///
/// @par MISRA C:2025 Rule 10.2
/// Essentially character types should only be added to/subtracted from
/// essentially signed or unsigned types, not other character types.
/// @par Category: Required
///
/// Example:
/// @code
///   char a = 'A', b = 'B';
///   char c = a + b;      // Warning: character + character
///   char d = a + 1;      // OK: character + integer
///   int diff = a - b;    // Warning: character - character
/// @endcode
class AvoidCharacterArithmeticCheck : public ClangTidyCheck {
public:
  AvoidCharacterArithmeticCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidCharacterArithmeticCheck() override = default;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isCharacterType(QualType Type) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCHARACTERARITHMETICCHECK_H

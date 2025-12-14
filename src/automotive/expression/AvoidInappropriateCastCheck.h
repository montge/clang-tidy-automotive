//===--- AvoidInappropriateCastCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINAPPROPRIATECASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINAPPROPRIATECASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects inappropriate casts between essential type categories.
///
/// Implements MISRA Rule 10.5: The value of an expression should not be cast
/// to an inappropriate essential type.
///
/// @par MISRA C:2025 Rule 10.5
/// Casts to inappropriate types indicate design issues.
/// @par Category: Advisory
///
/// Example:
/// @code
///   int x = 65;
///   char c = (char)x;     // Warning: signed to character cast
///   bool b = (bool)x;     // Warning: integer to boolean cast
/// @endcode
class AvoidInappropriateCastCheck : public ClangTidyCheck {
public:
  AvoidInappropriateCastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  enum class EssentialType {
    Boolean,
    Character,
    SignedInt,
    UnsignedInt,
    FloatingPoint,
    Enum,
    Other
  };

  EssentialType getEssentialType(QualType Type) const;
  bool isInappropriateCast(EssentialType From, EssentialType To) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINAPPROPRIATECASTCHECK_H

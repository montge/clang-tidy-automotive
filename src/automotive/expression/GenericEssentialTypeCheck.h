//===--- GenericEssentialTypeCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICESSENTIALTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICESSENTIALTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Checks that _Generic controlling expression has matching essential
/// and standard types.
///
/// The controlling expression of a generic selection shall have an essential
/// type that matches its standard type. This prevents unexpected selection
/// due to integer promotion.
///
/// Example:
/// @code
///   short s16;
///   // Non-compliant: s16+s16 has essential type short but standard type int
///   _Generic(s16 + s16, short: 1, int: 2, default: 0);
///
///   // Non-compliant: 'c' has essential type char but standard type int
///   _Generic('c', char: 1, int: 2, default: 0);
///
///   // Compliant: s16 has same essential and standard type
///   _Generic(s16, short: 1, default: 0);
/// @endcode
class GenericEssentialTypeCheck : public ClangTidyCheck {
public:
  GenericEssentialTypeCheck(StringRef Name, ClangTidyContext *Context)
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
  StringRef getEssentialTypeName(EssentialType ET) const;
  bool isIntegerConstantExpression(const Expr *E, ASTContext &Ctx) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICESSENTIALTYPECHECK_H

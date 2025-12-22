//===--- AvoidCharacterTypeMismatchCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCHARACTERTYPEMISMATCHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCHARACTERTYPEMISMATCHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects assignments between different character types.
///
/// MISRA C++:2023 Rule 7.0: A value of any of the standard character types
/// shall only be assigned to a variable of the same character type.
///
/// This ensures that char, signed char, unsigned char, wchar_t, char8_t,
/// char16_t, and char32_t are not mixed in assignments.
class AvoidCharacterTypeMismatchCheck : public ClangTidyCheck {
public:
  AvoidCharacterTypeMismatchCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

private:
  StringRef getCharTypeName(const QualType &QT) const;
  bool isCharacterType(const QualType &QT) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCHARACTERTYPEMISMATCHCHECK_H

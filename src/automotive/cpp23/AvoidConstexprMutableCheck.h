//===--- AvoidConstexprMutableCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTEXPRMUTABLECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTEXPRMUTABLECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-constexpr
/// @brief Detects constexpr variables with mutable sub-objects.
///
/// Constexpr variables are intended to be fully compile-time constants.
/// Having mutable members in constexpr variables violates this intent and
/// can lead to undefined behavior or unexpected modifications at runtime.
///
/// @par MISRA C++:2023 Rule 14.1
/// Constexpr variables should not have mutable sub-objects.
/// @par Category: Advisory
///
/// Example:
/// @code
///   struct S {
///     mutable int x;  // Mutable member
///     int y;
///   };
///
///   constexpr S obj{1, 2};  // Warning - constexpr variable with mutable
///   member
///
///   struct T {
///     int x;
///     int y;
///   };
///
///   constexpr T obj2{1, 2};  // OK - no mutable members
/// @endcode
class AvoidConstexprMutableCheck : public ClangTidyCheck {
public:
  AvoidConstexprMutableCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidConstexprMutableCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool hasMutableSubObject(QualType Type) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTEXPRMUTABLECHECK_H

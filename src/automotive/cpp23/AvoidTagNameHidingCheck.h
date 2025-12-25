//===--- AvoidTagNameHidingCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTAGNAMEHIDINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTAGNAMEHIDINGCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-declarations
/// @brief Detects when a tag name hides other identifiers in enclosing scopes.
///
/// A tag name (struct/class/enum/union) that has the same name as an
/// identifier in an enclosing scope can lead to confusion and bugs.
///
/// @par MISRA C++:2023 Rule 5.7 (Advisory)
/// A tag name should not hide other identifiers.
///
/// Example:
/// @code
///   int Point = 5;
///
///   void foo() {
///       struct Point {  // Non-compliant - tag name hides variable
///           int x, y;
///       };
///   }
///
///   // Compliant:
///   void bar() {
///       struct Location {  // Different name, no hiding
///           int x, y;
///       };
///   }
/// @endcode
class AvoidTagNameHidingCheck : public ClangTidyCheck {
public:
  AvoidTagNameHidingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTAGNAMEHIDINGCHECK_H

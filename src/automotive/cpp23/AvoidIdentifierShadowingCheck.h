//===--- AvoidIdentifierShadowingCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIDENTIFIERSHADOWINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIDENTIFIERSHADOWINGCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-declarations
/// @brief Detects identifier shadowing in nested scopes.
///
/// An identifier declared in an inner scope that has the same name as an
/// identifier in an enclosing scope can lead to confusion and bugs.
///
/// @par MISRA C++:2023 Rule 6.2 (Required)
/// An identifier declared in a local scope shall not have the same name
/// as an identifier in an enclosing scope.
///
/// Example:
/// @code
///   int x = 10;  // Outer scope
///
///   void foo() {
///       int x = 20;  // Non-compliant - shadows outer x
///   }
///
///   // Compliant:
///   void bar() {
///       int y = 30;  // Different name, no shadowing
///   }
/// @endcode
class AvoidIdentifierShadowingCheck : public ClangTidyCheck {
public:
  AvoidIdentifierShadowingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIDENTIFIERSHADOWINGCHECK_H

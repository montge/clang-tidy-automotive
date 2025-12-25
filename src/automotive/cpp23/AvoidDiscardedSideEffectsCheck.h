//===--- AvoidDiscardedSideEffectsCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDISCARDEDSIDEEFFECTSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDISCARDEDSIDEEFFECTSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects function calls with side effects where the return value is
/// discarded.
///
/// MISRA C++:2023 Rule 28.3 (Required): A function with side effects shall not
/// be called from a context where those side effects would be discarded.
///
/// This check detects:
/// - Calls to [[nodiscard]] functions where the return value is ignored
/// - Explicit cast to void of function calls: (void)important_func()
/// - Comma operator discarding function results: func1(), func2()
///
/// Examples:
/// @code
///   [[nodiscard]] int important_func();
///
///   // Violations:
///   important_func();           // ignored nodiscard return
///   (void)important_func();     // explicit void cast
///   important_func(), other();  // comma operator discards result
///
///   // Compliant:
///   int result = important_func();  // value used
///   if (important_func()) { }       // value used in condition
/// @endcode
class AvoidDiscardedSideEffectsCheck : public ClangTidyCheck {
public:
  AvoidDiscardedSideEffectsCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDISCARDEDSIDEEFFECTSCHECK_H

//===--- ObsolescentFeaturesCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_OBSOLESCENTFEATURESCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_OBSOLESCENTFEATURESCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of obsolescent language features in C.
///
/// MISRA C:2025 Rule 1.5 prohibits obsolescent language features including:
/// - K&R style function definitions
/// - Old-style function declarations
/// - Unprototyped functions
///
/// Example:
/// \code
///   // K&R style definition - obsolescent
///   int func(x, y)
///   int x;
///   int y;
///   { return x + y; }
///
///   // Prototype style - compliant
///   int func(int x, int y) { return x + y; }
/// \endcode
class ObsolescentFeaturesCheck : public ClangTidyCheck {
public:
  ObsolescentFeaturesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.C99 || LangOpts.C11 || LangOpts.C17 || LangOpts.C23;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_OBSOLESCENTFEATURESCHECK_H

//===--- AvoidUnionCppCheck.h - clang-tidy ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNIONCPPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNIONCPPCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects union type declarations and uses in C++ code.
///
/// Unions shall not be used in C++ code as they can lead to type-safety
/// issues and undefined behavior when accessing inactive members.
/// Use std::variant instead.
///
/// Related MISRA C++:2023 Rule 12.3
///
/// Example:
/// \code
///   union Data {     // Warning: union shall not be used
///     int i;
///     float f;
///   };
/// \endcode
class AvoidUnionCppCheck : public ClangTidyCheck {
public:
  AvoidUnionCppCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNIONCPPCHECK_H

//===--- AvoidCtimeCppCheck.h - clang-tidy ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCTIMECPPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCTIMECPPCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects use of the ctime header in C++ code.
///
/// The <ctime> header shall not be used. Use <chrono> instead for
/// type-safe time handling.
///
/// Related MISRA C++:2023 Rule 21.10
///
/// Example:
/// \code
///   #include <ctime>  // Warning: ctime shall not be used
///   time_t t = time(nullptr);  // Warning: time function from ctime
/// \endcode
class AvoidCtimeCppCheck : public ClangTidyCheck {
public:
  AvoidCtimeCppCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCTIMECPPCHECK_H

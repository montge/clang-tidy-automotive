//===--- AvoidAsmCheck.h - clang-tidy --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASMCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASMCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects the use of asm declarations.
///
/// The asm declaration provides a mechanism to embed assembly language code
/// directly in C++ programs. However, using inline assembly reduces code
/// portability, makes code harder to maintain, and can interfere with
/// compiler optimizations.
///
/// Related MISRA C++:2023 Rule 7.0.1
///
/// Example:
/// \code
///   asm("nop");                // Warning: asm declaration
///   __asm__("nop");            // Warning: GCC asm extension
///   __asm { nop }              // Warning: MSVC asm extension
/// \endcode
class AvoidAsmCheck : public ClangTidyCheck {
public:
  AvoidAsmCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDASMCHECK_H

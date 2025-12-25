//===--- AvoidExternInSourceFileCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDEXTERNINSOURCEFILECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDEXTERNINSOURCEFILECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects extern declarations in source files.
///
/// MISRA C:2025 Rule 8.19: There should be no external declarations in a
/// source file. External declarations (using extern keyword) should be placed
/// in header files, not in source files, to ensure consistency and avoid
/// duplicate/inconsistent declarations.
///
/// Example:
/// \code
///   // In foo.c - Non-compliant
///   extern int global_var;       // Warning: extern in source file
///   extern void other_func(void); // Warning: extern in source file
/// \endcode
class AvoidExternInSourceFileCheck : public ClangTidyCheck {
public:
  AvoidExternInSourceFileCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isSourceFile(StringRef FileName) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDEXTERNINSOURCEFILECHECK_H

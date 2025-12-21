//===--- AvoidImplicitNullComparisonCheck.h - clang-tidy --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIMPLICITNULLCOMPARISONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIMPLICITNULLCOMPARISONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects implicit comparisons of pointers to NULL.
///
/// Pointers shall not be implicitly compared to NULL. Explicit comparison
/// makes the intent clearer and avoids potential confusion.
///
/// Related MISRA C:2025 Rule 11.11
///
/// Example:
/// \code
///   if (ptr) { ... }        // Warning: implicit NULL comparison
///   if (ptr != NULL) { ... } // OK: explicit comparison
/// \endcode
class AvoidImplicitNullComparisonCheck : public ClangTidyCheck {
public:
  AvoidImplicitNullComparisonCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDIMPLICITNULLCOMPARISONCHECK_H

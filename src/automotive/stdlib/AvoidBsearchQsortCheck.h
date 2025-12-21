//===--- AvoidBsearchQsortCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBSEARCHQSORTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBSEARCHQSORTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects calls to bsearch and qsort from stdlib.h.
///
/// The bsearch and qsort functions shall not be used because their comparison
/// function callback can lead to undefined behavior if not implemented
/// correctly, and they have unspecified behavior regarding equal elements.
///
/// Related MISRA C:2025 Rule 21.9
///
/// Example:
/// \code
///   qsort(arr, n, sizeof(int), cmp);  // Warning: qsort shall not be used
///   bsearch(&key, arr, n, sizeof(int), cmp);  // Warning: bsearch
/// \endcode
class AvoidBsearchQsortCheck : public ClangTidyCheck {
public:
  AvoidBsearchQsortCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBSEARCHQSORTCHECK_H

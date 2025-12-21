//===--- AvoidAtomicVoidCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDATOMICVOIDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDATOMICVOIDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects use of _Atomic qualifier on the incomplete type void.
///
/// The _Atomic qualifier shall not be applied to void because void is an
/// incomplete type and atomic operations require complete types.
///
/// Related MISRA C:2025 Rule 11.10
///
/// Example:
/// \code
///   _Atomic void *p;  // Warning: _Atomic applied to void
/// \endcode
class AvoidAtomicVoidCheck : public ClangTidyCheck {
public:
  AvoidAtomicVoidCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDATOMICVOIDCHECK_H

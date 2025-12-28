//===--- AvoidUninitializedAtomicCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNINITIALIZEDATOMICCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNINITIALIZEDATOMICCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects atomic objects that are not appropriately initialized before
/// being accessed.
///
/// MISRA C:2025 Rule 9.7 (Mandatory):
/// Atomic objects shall be appropriately initialized before being accessed.
///
/// Atomic objects must be initialized using ATOMIC_VAR_INIT or atomic_init
/// before they can be safely used.
///
/// Example:
/// \code
///   _Atomic int a;
///   a = 5;  // Warning: uninitialized atomic access
///
///   _Atomic int b = ATOMIC_VAR_INIT(0);  // OK
/// \endcode
class AvoidUninitializedAtomicCheck : public ClangTidyCheck {
public:
  AvoidUninitializedAtomicCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNINITIALIZEDATOMICCHECK_H

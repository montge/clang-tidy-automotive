//===--- ThreadJoinDetachCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADJOINDETACHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADJOINDETACHCHECK_H

#include "../../ClangTidyCheck.h"
#include <map>

namespace clang::tidy::automotive {

/// Detects obvious thread lifecycle violations.
///
/// MISRA C:2025 Rule 22.11: A thread that was previously joined or detached
/// shall not be subsequently joined or detached.
///
/// This check detects:
/// - Double join on the same thread
/// - Double detach on the same thread
/// - Join after detach (or vice versa)
///
/// Note: This is a conservative check that only catches obvious violations
/// within the same function. Cross-function analysis is not performed.
class ThreadJoinDetachCheck : public ClangTidyCheck {
public:
  ThreadJoinDetachCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADJOINDETACHCHECK_H

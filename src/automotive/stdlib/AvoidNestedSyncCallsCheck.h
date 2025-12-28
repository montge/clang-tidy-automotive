//===--- AvoidNestedSyncCallsCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNESTEDSYNCCALLSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNESTEDSYNCCALLSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects nested calls to thread synchronization functions.
///
/// MISRA C:2025 Rule 22.20 (Mandatory):
/// Calls to thread synchronization functions shall not be nested.
///
/// Thread synchronization functions (mutex lock/unlock, condition variable
/// wait/signal) should not be called from within signal handlers or other
/// synchronization primitives to avoid deadlocks.
///
/// Example:
/// \code
///   mtx_lock(&mutex1);
///   mtx_lock(&mutex2);  // Warning: nested synchronization calls
///   mtx_unlock(&mutex2);
///   mtx_unlock(&mutex1);
/// \endcode
class AvoidNestedSyncCallsCheck : public ClangTidyCheck {
public:
  AvoidNestedSyncCallsCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isSyncFunction(StringRef FuncName) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNESTEDSYNCCALLSCHECK_H

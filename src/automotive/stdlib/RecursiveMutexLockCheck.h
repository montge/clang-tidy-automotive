//===--- RecursiveMutexLockCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_RECURSIVEMUTEXLOCKCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_RECURSIVEMUTEXLOCKCHECK_H

#include "../../ClangTidyCheck.h"
#include <map>
#include <set>

namespace clang::tidy::automotive {

/// Detects recursive locking of non-recursive mutexes within a function.
///
/// MISRA C:2025 Rule 22.18: Non-recursive mutexes shall not be recursively
/// locked. This check detects obvious cases where the same mutex is locked
/// twice without an intervening unlock within the same function.
///
/// Example:
/// \code
///   mtx_t mtx;
///   mtx_lock(&mtx);
///   mtx_lock(&mtx);  // Warning: recursive lock on non-recursive mutex
/// \endcode
class RecursiveMutexLockCheck : public ClangTidyCheck {
public:
  RecursiveMutexLockCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  // Track locked mutexes per function to detect double-locks
  std::map<const FunctionDecl *, std::set<std::string>> LockedMutexes;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_RECURSIVEMUTEXLOCKCHECK_H

//===--- MutexUnlockThreadCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MUTEXUNLOCKTHREADCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MUTEXUNLOCKTHREADCHECK_H

#include "../../ClangTidyCheck.h"
#include <map>
#include <string>

namespace clang::tidy::automotive {

/// Detects potential mutex unlock from different thread than lock.
///
/// MISRA C:2025 Rule 22.17: A mutex shall be unlocked from the same thread
/// that locked it.
///
/// This check attempts to detect obvious cases where a mutex might be
/// unlocked from a different thread than the one that locked it. Full
/// enforcement requires runtime analysis.
///
/// Example:
/// \code
///   mtx_t mtx;
///   void thread1() { mtx_lock(&mtx); }
///   void thread2() { mtx_unlock(&mtx); }  // Warning: potential cross-thread unlock
/// \endcode
///
/// Limitations:
/// - Only detects obvious patterns within translation unit
/// - Cannot track actual thread execution at runtime
/// - Serves primarily as a reminder to verify lock/unlock pairing
class MutexUnlockThreadCheck : public ClangTidyCheck {
public:
  MutexUnlockThreadCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

private:
  struct MutexLockInfo {
    std::string FunctionName;
    SourceLocation LockLoc;
  };

  std::map<std::string, MutexLockInfo> MutexLocks;
  const SourceManager *SM = nullptr;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MUTEXUNLOCKTHREADCHECK_H

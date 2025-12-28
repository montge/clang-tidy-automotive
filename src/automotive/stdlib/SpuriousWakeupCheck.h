//===--- SpuriousWakeupCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_SPURIOUSWAKEUPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_SPURIOUSWAKEUPCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects improper handling of spurious wakeups in condition variable waits.
///
/// MISRA C:2025 Rule 22.20: Spurious wakeups shall be handled correctly.
///
/// Condition variable wait functions (cnd_wait, pthread_cond_wait) can
/// experience spurious wakeups where the thread is awakened without an
/// actual signal. The wait should always be in a loop that rechecks the
/// predicate condition.
///
/// Example (violation):
/// \code
///   cnd_wait(&cond, &mutex);  // Wrong: not in a loop
///   // process data
/// \endcode
///
/// Example (compliant):
/// \code
///   while (!data_ready) {
///     cnd_wait(&cond, &mutex);  // Correct: in a loop with predicate check
///   }
///   // process data
/// \endcode
///
/// Note: This check looks for condition variable waits that are not
/// enclosed in a loop construct.
class SpuriousWakeupCheck : public ClangTidyCheck {
public:
  SpuriousWakeupCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_SPURIOUSWAKEUPCHECK_H

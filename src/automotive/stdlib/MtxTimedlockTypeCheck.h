//===--- MtxTimedlockTypeCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MTXTIMEDLOCKTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MTXTIMEDLOCKTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects incorrect usage of mtx_timedlock with inappropriate mutex types.
///
/// MISRA C:2025 Rule 21.26 (Required)
/// The Standard Library function mtx_timedlock() shall only be invoked on
/// mutex objects of appropriate mutex type.
///
/// The function mtx_timedlock() blocks the current thread until the mutex
/// pointed to is locked or until a specified timeout expires. However, if the
/// current thread has already locked the mutex and the mutex is not recursive,
/// or if the mutex does not support timeout, then the behavior is undefined.
///
/// The first argument of mtx_timedlock() must be a mutex object of type:
/// - mtx_timed
/// - mtx_timed | mtx_recursive
///
/// Invalid mutex types for mtx_timedlock():
/// - mtx_plain (does not support timeout)
/// - mtx_plain | mtx_recursive (does not support timeout)
///
/// Example:
/// \code
///   #include <threads.h>
///
///   mtx_t mutex_plain;
///   mtx_t mutex_timed;
///   mtx_t mutex_timed_recursive;
///
///   void init() {
///     mtx_init(&mutex_plain, mtx_plain);  // No timeout support
///     mtx_init(&mutex_timed, mtx_timed);  // Timeout supported
///     mtx_init(&mutex_timed_recursive, mtx_timed | mtx_recursive);
///   }
///
///   void example() {
///     struct timespec ts;
///
///     // Violation: mtx_timedlock with mtx_plain
///     mtx_timedlock(&mutex_plain, &ts);
///
///     // Compliant: mtx_timedlock with mtx_timed
///     mtx_timedlock(&mutex_timed, &ts);
///
///     // Compliant: mtx_timedlock with mtx_timed | mtx_recursive
///     mtx_timedlock(&mutex_timed_recursive, &ts);
///   }
/// \endcode
class MtxTimedlockTypeCheck : public ClangTidyCheck {
public:
  MtxTimedlockTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

private:
  // Track mutex types from mtx_init calls
  llvm::DenseMap<const VarDecl *, int> MutexTypes;

  // Helper to extract mutex type from mtx_init call
  int getMutexTypeFromInit(const CallExpr *InitCall, ASTContext *Ctx) const;

  // Check if mutex type supports timeout
  bool supportsTimeout(int MutexType) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MTXTIMEDLOCKTYPECHECK_H

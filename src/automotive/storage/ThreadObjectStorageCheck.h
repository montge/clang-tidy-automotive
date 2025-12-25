//===--- ThreadObjectStorageCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADOBJECTSTORAGECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADOBJECTSTORAGECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @brief Checks that thread objects have static storage duration.
///
/// Thread objects (thrd_t, mtx_t, cnd_t, tss_t) shall not have automatic
/// storage duration nor thread storage duration. They should be static
/// or have allocated storage.
///
/// Example:
/// @code
///   mtx_t global_mutex;  // Compliant: file scope (static)
///
///   void func(void) {
///     mtx_t local_mutex; // Non-compliant: automatic storage
///     static mtx_t ok;   // Compliant: static storage
///   }
/// @endcode
class ThreadObjectStorageCheck : public ClangTidyCheck {
public:
  ThreadObjectStorageCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADOBJECTSTORAGECHECK_H

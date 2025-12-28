//===--- TssUsageCheck.h - clang-tidy ---------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TSSUSAGECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TSSUSAGECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects improper usage of thread-specific storage (TSS) functions.
///
/// MISRA C:2025 Rule 21.23 (Required)
/// Thread-specific storage (TSS) functions from <threads.h> require careful
/// handling to avoid undefined behavior and resource leaks.
///
/// This check ensures that:
/// - tss_create is properly paired with tss_delete
/// - tss_get and tss_set are used with valid TSS objects
/// - TSS destructors are properly defined
///
/// Example:
/// \code
///   #include <threads.h>
///
///   void cleanup(void* data) {
///     free(data);
///   }
///
///   tss_t key;
///
///   void init() {
///     // Compliant: tss_create with destructor
///     if (tss_create(&key, cleanup) != thrd_success) {
///       // handle error
///     }
///   }
///
///   void cleanup_func() {
///     // Compliant: tss_delete paired with tss_create
///     tss_delete(key);
///   }
///
///   void use_tss() {
///     void* data = malloc(sizeof(int));
///     // Compliant: tss_set with valid key
///     tss_set(key, data);
///
///     // Compliant: tss_get with valid key
///     void* retrieved = tss_get(key);
///   }
/// \endcode
class TssUsageCheck : public ClangTidyCheck {
public:
  TssUsageCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isTssFunction(StringRef FuncName) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TSSUSAGECHECK_H

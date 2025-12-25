//===--- MemorySyncOrderCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MEMORYSYNCORDERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MEMORYSYNCORDERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects use of non-sequentially-consistent memory ordering in atomic
/// operations.
///
/// MISRA C:2025 Rule 21.25 (Required)
/// All memory synchronization operations shall be executed in sequentially
/// consistent order.
///
/// This check ensures that all atomic operations use memory_order_seq_cst
/// to maintain predictable memory ordering semantics. Relaxed memory ordering
/// can lead to subtle bugs and is difficult to reason about in safety-critical
/// code.
///
/// Example:
/// \code
///   #include <stdatomic.h>
///   atomic_int counter;
///
///   // Violation: using relaxed memory ordering
///   atomic_load_explicit(&counter, memory_order_relaxed);
///
///   // Compliant: using sequentially consistent ordering
///   atomic_load_explicit(&counter, memory_order_seq_cst);
///
///   // Compliant: implicit seq_cst ordering
///   atomic_load(&counter);
/// \endcode
class MemorySyncOrderCheck : public ClangTidyCheck {
public:
  MemorySyncOrderCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isNonSeqCstMemoryOrder(const Expr *E) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MEMORYSYNCORDERCHECK_H

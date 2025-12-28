//===--- UnsynchronizedAccessCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNSYNCHRONIZEDACCESSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNSYNCHRONIZEDACCESSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects potential unsynchronized access to shared objects.
///
/// MISRA C:2025 Rule 22.16: An object shall not be accessed from competing
/// threads without synchronization.
///
/// This check provides a basic detection of global/static variables that
/// might be accessed from multiple threads without proper synchronization
/// mechanisms (mutexes, atomics, etc.).
///
/// Limitations:
/// - Cannot perform full inter-thread dataflow analysis
/// - Reports potential issues that require manual review
/// - Detects obvious patterns but may have false positives/negatives
///
/// Example:
/// \code
///   int global_counter = 0;  // Warning: potentially shared across threads
///
///   void increment() {
///     global_counter++;  // Unsynchronized access
///   }
/// \endcode
///
/// Note: This is a heuristic-based check. Full enforcement requires
/// runtime analysis or formal verification tools.
class UnsynchronizedAccessCheck : public ClangTidyCheck {
public:
  UnsynchronizedAccessCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.C11 || LangOpts.CPlusPlus11;
  }
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNSYNCHRONIZEDACCESSCHECK_H

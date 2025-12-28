//===--- ThreadSpecificStorageCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADSPECIFICSTORAGECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADSPECIFICSTORAGECHECK_H

#include "../../ClangTidyCheck.h"
#include <map>
#include <string>

namespace clang::tidy::automotive {

/// Detects potential misuse of thread-specific storage.
///
/// MISRA C:2025 Rule 22.15: Thread-specific storage pointers shall be
/// retrieved from the same thread that created them.
///
/// This check detects uses of tss_get/pthread_getspecific without proper
/// thread context verification. While full inter-thread analysis is not
/// feasible, this check warns about obvious misuses.
///
/// Example:
/// \code
///   tss_t key;
///   // Warning: no guarantee tss_get is called from correct thread
///   void* ptr = tss_get(key);
/// \endcode
///
/// Note: This is a conservative check with limited scope. It primarily
/// serves as a reminder to verify thread-specific storage usage.
class ThreadSpecificStorageCheck : public ClangTidyCheck {
public:
  ThreadSpecificStorageCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

private:
  struct TSSInfo {
    SourceLocation CreateLoc;
    std::string ThreadContext;
  };

  std::map<std::string, TSSInfo> TSSKeys;
  const SourceManager *SM = nullptr;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_THREADSPECIFICSTORAGECHECK_H

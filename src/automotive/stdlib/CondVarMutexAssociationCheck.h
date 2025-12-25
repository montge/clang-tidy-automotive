//===--- CondVarMutexAssociationCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CONDVARMUTEXASSOCIATIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CONDVARMUTEXASSOCIATIONCHECK_H

#include "../../ClangTidyCheck.h"
#include <map>
#include <set>

namespace clang::tidy::automotive {

/// Detects condition variables used with multiple different mutexes.
///
/// MISRA C:2025 Rule 22.19: A condition variable shall be associated with
/// at most one mutex object. Using the same condition variable with different
/// mutexes can lead to race conditions and undefined behavior.
///
/// Example:
/// \code
///   cnd_t cv;
///   mtx_t mtx1, mtx2;
///   cnd_wait(&cv, &mtx1);
///   cnd_wait(&cv, &mtx2);  // Warning: cv used with different mutex
/// \endcode
class CondVarMutexAssociationCheck : public ClangTidyCheck {
public:
  CondVarMutexAssociationCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

private:
  // Map condition variable name to set of (mutex name, location) pairs
  struct MutexUsage {
    std::string MutexName;
    SourceLocation Loc;
  };
  std::map<std::string, std::vector<MutexUsage>> CondVarMutexMap;
  const SourceManager *SM = nullptr;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CONDVARMUTEXASSOCIATIONCHECK_H

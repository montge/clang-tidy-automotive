//===--- WellFormedSwitchCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATEMENT_WELLFORMEDSWITCHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATEMENT_WELLFORMEDSWITCHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects switch statements that are not well-formed according to MISRA.
///
/// MISRA C:2025 Rule 16.1: All switch statements shall be well-formed.
/// - Every non-empty case shall end with a break, continue, return, or throw
/// - Every switch shall have a default clause
class WellFormedSwitchCheck : public ClangTidyCheck {
public:
  WellFormedSwitchCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool hasTerminatingStatement(const Stmt *S, const ASTContext &Context) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATEMENT_WELLFORMEDSWITCHCHECK_H

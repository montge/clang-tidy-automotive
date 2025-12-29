//===--- MissingNestedBracesCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGNESTEDBRACESCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGNESTEDBRACESCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects missing braces in nested aggregate initialization.
///
/// MISRA C:2012 Rule 9.2 requires that the initializer for an aggregate
/// or union shall be enclosed in braces. This specifically means nested
/// aggregates must have explicit braces.
///
/// Example of non-compliant code:
/// \code
///   struct Nested { struct Point { int x; int y; } p; int z; };
///   struct Nested n = {1, 2, 3};  // Missing braces around {1, 2}
/// \endcode
///
/// Example of compliant code:
/// \code
///   struct Nested n = {{1, 2}, 3};  // Explicit braces for nested Point
/// \endcode
class MissingNestedBracesCheck : public ClangTidyCheck {
public:
  MissingNestedBracesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~MissingNestedBracesCheck() override = default;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGNESTEDBRACESCHECK_H

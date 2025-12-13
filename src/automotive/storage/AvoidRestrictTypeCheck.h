//===--- AvoidRestrictTypeCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESTRICTTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESTRICTTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of the restrict type qualifier.
///
/// The restrict keyword is a C99 feature that provides optimization hints
/// to the compiler about pointer aliasing. However, incorrect use of restrict
/// can lead to undefined behavior if the aliasing guarantees are violated.
/// Its use should be carefully considered and limited to performance-critical
/// code where aliasing behavior is well-understood.
///
/// Related MISRA C:2025 Rule: 8.14 - The restrict type qualifier shall not
/// be used.
///
/// Example:
/// \code
///   void func(int *restrict ptr) { }  // Warning: restrict used
/// \endcode
class AvoidRestrictTypeCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidRestrictTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for restrict qualified types.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched restrict qualifiers and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDRESTRICTTYPECHECK_H

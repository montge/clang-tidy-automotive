//===--- MissingStaticInternalLinkageCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGSTATICINTERNALLINKAGECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGSTATICINTERNALLINKAGECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects objects and functions with internal linkage missing static specifier.
///
/// Objects and functions that could have internal linkage (i.e., are only
/// used within a single translation unit) should be declared static. This
/// prevents namespace pollution, reduces the risk of name collisions, and
/// allows better compiler optimizations.
///
/// Related MISRA C:2025 Rule: 8.7 - Functions and objects should not be
/// defined with external linkage if they are referenced in only one
/// translation unit.
///
/// Example:
/// \code
///   int helper(void) { return 0; }  // Warning: could be static
/// \endcode
class MissingStaticInternalLinkageCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  MissingStaticInternalLinkageCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for function and object declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched declarations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGSTATICINTERNALLINKAGECHECK_H

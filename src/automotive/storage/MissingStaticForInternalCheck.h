//===--- MissingStaticForInternalCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGSTATICFORINTERNALCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGSTATICFORINTERNALCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects functions and objects that could be declared static.
///
/// MISRA C:2025 Rule 8.7 (Advisory) recommends that functions and objects
/// that are only referenced within a single translation unit should be
/// declared with internal linkage (static). This reduces the risk of
/// naming collisions and improves encapsulation.
///
/// Example of non-compliant code:
/// \code
///   // In file.c - helper function only used in this file
///   int helper_function(int x) { return x * 2; }
///   int public_function(void) { return helper_function(21); }
/// \endcode
///
/// Example of compliant code:
/// \code
///   static int helper_function(int x) { return x * 2; }
///   int public_function(void) { return helper_function(21); }
/// \endcode
///
/// Note: This check flags functions and file-scope objects without the
/// static keyword that have no external declaration. It cannot perform
/// whole-program analysis to verify they're truly unused externally.
class MissingStaticForInternalCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  MissingStaticForInternalCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for function and variable declarations.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched declarations and emits diagnostics.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGSTATICFORINTERNALCHECK_H

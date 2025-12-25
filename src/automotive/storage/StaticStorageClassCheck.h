//===--- StaticStorageClassCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATICSTORAGECLASS_CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATICSTORAGECLASS_CHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects declarations with internal linkage that are missing the static
/// storage class specifier.
///
/// MISRA C:2025 Rule 8.8 (Required): The static storage class specifier shall
/// be used in all declarations of objects and functions that have internal
/// linkage.
///
/// When a function or object has internal linkage (i.e., is only visible
/// within its translation unit), all declarations of that entity must include
/// the static keyword to make the internal linkage explicit.
///
/// Example of non-compliant code:
/// \code
///   static int helper(void);    // First declaration with static
///   int helper(void) { return 0; }  // Violation: redeclaration missing static
/// \endcode
///
/// Example of compliant code:
/// \code
///   static int helper(void);    // First declaration with static
///   static int helper(void) { return 0; }  // OK: both have static
/// \endcode
///
/// This check verifies that if any declaration of a function or variable
/// uses the static storage class specifier, all declarations must use it.
class StaticStorageClassCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  StaticStorageClassCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~StaticStorageClassCheck() override = default;

  /// Registers AST matchers for function and variable declarations.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched declarations and emits diagnostics.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATICSTORAGECLASS_CHECK_H

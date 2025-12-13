//===--- StaticInlineCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATICINLINECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATICINLINECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects inline functions that lack the static storage class specifier.
///
/// Inline functions should be declared static to ensure they have internal
/// linkage and avoid potential multiple definition errors when included in
/// multiple translation units. Without static, inline functions can lead to
/// ODR (One Definition Rule) violations and linker issues.
///
/// Related MISRA C:2025 Rule: 8.9 - An inline function shall be declared with
/// the static storage class.
///
/// Example:
/// \code
///   inline int func(void) { return 0; }  // Warning: missing static
///   static inline int foo(void) { return 1; }  // OK
/// \endcode
class StaticInlineCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  StaticInlineCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for inline function declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched inline functions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_STATICINLINECHECK_H

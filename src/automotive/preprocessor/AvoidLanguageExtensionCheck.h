//===--- AvoidLanguageExtensionCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLANGUAGEEXTENSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLANGUAGEEXTENSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of compiler-specific language extensions.
///
/// MISRA C:2025 Rule 1.2 (Advisory) recommends that language extensions
/// should not be used, as they reduce code portability across different
/// compilers and platforms.
///
/// This check detects common GCC/Clang extensions including:
/// - __attribute__ specifiers
/// - __typeof__ and typeof expressions
/// - __extension__ keyword
/// - Statement expressions ({ ... })
/// - Compound literals in non-standard contexts
///
/// Example of non-compliant code:
/// \code
///   int x __attribute__((aligned(16)));
///   __typeof__(x) y;
/// \endcode
class AvoidLanguageExtensionCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  AvoidLanguageExtensionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for extension constructs.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched extensions and emits diagnostics.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLANGUAGEEXTENSIONCHECK_H

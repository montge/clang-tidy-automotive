//===--- FunctionDeclarationMismatchCheck.h - clang-tidy --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FUNCTIONDECLARATIONMISMATCHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FUNCTIONDECLARATIONMISMATCHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects mismatches between function declarations and definitions.
///
/// Function declarations and their corresponding definitions must match
/// exactly in terms of return type, parameter types, and qualifiers. Mismatches
/// can lead to undefined behavior, linking errors, or subtle bugs that are
/// difficult to diagnose, especially in multi-file projects.
///
/// Related MISRA C:2025 Rule: 8.3 - All declarations and definitions of an
/// object or function shall use compatible types.
///
/// Example:
/// \code
///   void func(int x);       // Declaration
///   void func(long x) {}    // Warning: parameter type mismatch
/// \endcode
class FunctionDeclarationMismatchCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  FunctionDeclarationMismatchCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~FunctionDeclarationMismatchCheck() override = default;

  /// Registers AST matchers for function declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched function declarations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FUNCTIONDECLARATIONMISMATCHCHECK_H

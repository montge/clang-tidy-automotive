//===--- ImplicitIntCheck.h - clang-tidy ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IMPLICITINTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IMPLICITINTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects declarations that implicitly use the int type.
///
/// In older C standards, omitting a type specifier defaults to int. This
/// implicit typing is error-prone and makes code less readable. All
/// declarations should explicitly specify their type to improve clarity
/// and prevent misunderstandings about the intended type.
///
/// Related MISRA C:2025 Rule: 8.1 - Types shall be explicitly specified.
///
/// Example:
/// \code
///   const x = 5;        // Warning: implicit int
///   unsigned y = 10;    // Warning: implicit int (unsigned int intended)
/// \endcode
class ImplicitIntCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  ImplicitIntCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~ImplicitIntCheck() override = default;

  /// Registers AST matchers for variable and function declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched declarations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Checks variable declaration for implicit int.
  void checkVarDecl(const VarDecl *VD, const SourceManager &SM,
                    const LangOptions &LangOpts);

  /// Checks function declaration for implicit int return type.
  void checkFuncDecl(const FunctionDecl *FD, const SourceManager &SM,
                     const LangOptions &LangOpts);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IMPLICITINTCHECK_H

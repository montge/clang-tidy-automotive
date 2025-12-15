//===--- ImplicitFunctionDeclCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IMPLICITFUNCTIONDECLCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IMPLICITFUNCTIONDECLCHECK_H

#include "../../ClangTidyCheck.h"
#include "../utils/Utils.h"

namespace clang::tidy::automotive {

/// Detects calls to functions without prior declarations.
///
/// In C90, calling a function without a prior declaration causes the compiler
/// to implicitly declare it with default return type int. This can lead to
/// type mismatches, runtime errors, and undefined behavior if the actual
/// function signature differs from the implicit declaration.
///
/// Related MISRA C:2025 Rule: 8.1 - Functions shall have prototypes and the
/// prototype shall be visible at the point of call.
///
/// Example:
/// \code
///   int main(void) {
///     foo();  // Warning: implicit function declaration (C90)
///   }
/// \endcode
class ImplicitFunctionDeclCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  ImplicitFunctionDeclCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~ImplicitFunctionDeclCheck() override = default;

  /// Registers AST matchers for function calls.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched function calls and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

  /// Only applies to C90 language mode.
  /// \param LangOpts The language options for the current translation unit.
  /// \return true if the language is C90.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return utils::isLanguageC90(LangOpts);
  }
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IMPLICITFUNCTIONDECLCHECK_H

//===--- DefaultArgumentOrderCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_DEFAULTARGUMENTORDERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_DEFAULTARGUMENTORDERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-functions
/// @brief Detects inconsistent default argument values in function
/// redeclarations.
///
/// MISRA C++:2023 Rule 8.5.1 requires that if a function has a parameter with
/// a default argument, all subsequent parameters shall have default arguments.
/// While this is enforced by the compiler, this check also detects when default
/// arguments are redefined in redeclarations with different values, which can
/// lead to confusion and maintenance issues.
///
/// @par MISRA C++:2023 Rule 8.5.1
/// If a function has a parameter with a default argument, all subsequent
/// parameters shall have default arguments.
/// @par Category: Required
///
/// Example:
/// @code
///   // Declaration with default argument
///   void foo(int x, int y = 10);
///
///   // Redeclaration with different default value - Warning
///   void foo(int x, int y = 20);
///
///   // Definition
///   void foo(int x, int y) { /* ... */ }
/// @endcode
class DefaultArgumentOrderCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  DefaultArgumentOrderCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~DefaultArgumentOrderCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for function declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched function declarations and checks for default argument
  /// issues.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_DEFAULTARGUMENTORDERCHECK_H

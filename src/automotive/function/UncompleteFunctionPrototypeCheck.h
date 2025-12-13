//===--- FunctionPrototypeWithNamedParametersCheck.h - clang-tidy *- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNCOMPLETEFUNCTIONPROTOTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNCOMPLETEFUNCTIONPROTOTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects function prototypes without complete parameter type information.
///
/// Function prototypes should include complete parameter type information.
/// Old-style K&R function declarations or prototypes with empty parameter
/// lists provide no type checking for arguments, which can lead to type
/// mismatches and undefined behavior at call sites.
///
/// Related MISRA C:2025 Rule: 8.2 - Function types shall be in prototype form
/// with named parameters.
///
/// Example:
/// \code
///   int func();      // Warning: incomplete prototype (no parameter info)
///   int foo(a, b);   // Warning: K&R style declaration
/// \endcode
class UncompleteFunctionPrototypeCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  UncompleteFunctionPrototypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for function declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched function declarations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNCOMPLETEFUNCTIONPROTOTYPECHECK_H

//===--- TgmathOperandTypeCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TGMATHOPERANDTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TGMATHOPERANDTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects inappropriate operand types passed to type-generic macros in
/// <tgmath.h>.
///
/// MISRA C:2025 Rule 21.22 (Mandatory)
/// All operand arguments to type-generic macros in <tgmath.h> shall have an
/// appropriate essential type.
///
/// The type-generic macros in <tgmath.h> require floating-point or complex
/// types as arguments. Passing integer types or other inappropriate types
/// leads to undefined behavior or implementation-defined results.
///
/// Example:
/// \code
///   #include <tgmath.h>
///   int x = 5;
///   double y = sin(x);  // Violation: integer argument to type-generic macro
///   double z = sin(5.0); // Compliant: floating-point argument
/// \endcode
class TgmathOperandTypeCheck : public ClangTidyCheck {
public:
  TgmathOperandTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isTgmathMacro(StringRef Name) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TGMATHOPERANDTYPECHECK_H

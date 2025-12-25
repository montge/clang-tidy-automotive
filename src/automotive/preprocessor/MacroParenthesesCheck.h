//===--- MacroParenthesesCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MACROPARENTHESESCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MACROPARENTHESESCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// MISRA C:2025 Rule 20.7 - Expressions resulting from macro expansion shall
/// be appropriately delimited.
///
/// This rule requires that macro parameters used in expressions are properly
/// parenthesized to avoid operator precedence issues.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/automotive/c25-req-20.7.html
class MacroParenthesesCheck : public ClangTidyCheck {
public:
  MacroParenthesesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MACROPARENTHESESCHECK_H

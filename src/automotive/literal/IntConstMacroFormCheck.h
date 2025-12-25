//===--- IntConstMacroFormCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_INTCONSTMACROFORMCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_INTCONSTMACROFORMCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// MISRA C:2025 Rule 7.5 - The argument of an integer constant macro shall
/// have an appropriate form.
///
/// Integer constant macros (INT8_C, UINT16_C, INT32_C, UINT64_C, etc.) from
/// <stdint.h> should have arguments that:
/// 1. Are simple integer constants (not expressions)
/// 2. Do not have type suffixes that conflict with the macro's purpose
/// 3. Are within the valid range for the target type
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/automotive/c25-mand-7.5.html
class IntConstMacroFormCheck : public ClangTidyCheck {
public:
  IntConstMacroFormCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_INTCONSTMACROFORMCHECK_H

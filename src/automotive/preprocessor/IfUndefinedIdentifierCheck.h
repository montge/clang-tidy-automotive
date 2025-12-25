//===--- IfUndefinedIdentifierCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IFUNDEFINEDIDENTIFIERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IFUNDEFINEDIDENTIFIERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects undefined identifiers in #if and #elif preprocessing directives.
///
/// All identifiers used in the controlling expression of #if or #elif
/// preprocessing directives shall be #define'd before evaluation, or be
/// the operand of a defined operator.
///
/// When an undefined identifier appears in a preprocessor conditional
/// expression, it is replaced by 0, which can lead to unexpected behavior
/// and difficult-to-diagnose bugs.
///
/// Related MISRA C:2025 Rule: 20.9
///
/// Example violations:
/// \code
///   #if UNDEFINED_MACRO        // Warning: UNDEFINED_MACRO not defined
///   #if UNDEFINED > 0          // Warning: UNDEFINED not defined
/// \endcode
///
/// Example compliant code:
/// \code
///   #if defined(MACRO)         // OK: using defined operator
///   #define MACRO 1
///   #if MACRO                  // OK: MACRO is defined
///   #ifdef MACRO               // OK: implicit defined check
/// \endcode
class IfUndefinedIdentifierCheck : public ClangTidyCheck {
public:
  IfUndefinedIdentifierCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IFUNDEFINEDIDENTIFIERCHECK_H

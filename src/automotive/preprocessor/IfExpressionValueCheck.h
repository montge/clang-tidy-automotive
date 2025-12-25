//===--- IfExpressionValueCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IFEXPRESSIONVALUECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IFEXPRESSIONVALUECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects #if and #elif preprocessor directives whose controlling expression
/// does not evaluate to 0 or 1.
///
/// The controlling expression of a #if or #elif preprocessing directive shall
/// evaluate to 0 or 1. This ensures that conditions are boolean in nature,
/// improving code clarity and avoiding unexpected behavior from non-boolean
/// values.
///
/// Related MISRA C:2025 Rule: 20.8 (Required)
///
/// Examples of violations:
/// \code
///   #if 2                    // Non-compliant: evaluates to 2
///   #if VERSION              // Non-compliant if VERSION is not 0 or 1
///   #if MAX_SIZE - 1         // Non-compliant: arbitrary value
/// \endcode
///
/// Examples of compliant code:
/// \code
///   #if 0                    // Compliant
///   #if 1                    // Compliant
///   #if defined(FEATURE)     // Compliant: defined() yields 0 or 1
///   #if X > 0                // Compliant: comparison yields 0 or 1
///   #if FLAG                 // Compliant if FLAG is defined as 0 or 1
/// \endcode
class IfExpressionValueCheck : public ClangTidyCheck {
public:
  IfExpressionValueCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_IFEXPRESSIONVALUECHECK_H

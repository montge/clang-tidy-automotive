//===--- AvoidCrossFileIfCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCROSSFILEIFCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCROSSFILEIFCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects #if/#ifdef/#ifndef directives whose #endif is in a different file.
///
/// All #else, #elif and #endif preprocessor directives shall reside in the
/// same file as the #if, #ifdef or #ifndef directive to which they are
/// related.
///
/// Related MISRA C:2025 Rule: 20.14
///
/// Example:
/// \code
///   // In header.h:
///   #if FEATURE_ENABLED
///   // In source.c that includes header.h:
///   #endif  // Warning: #endif in different file from #if
/// \endcode
class AvoidCrossFileIfCheck : public ClangTidyCheck {
public:
  AvoidCrossFileIfCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCROSSFILEIFCHECK_H

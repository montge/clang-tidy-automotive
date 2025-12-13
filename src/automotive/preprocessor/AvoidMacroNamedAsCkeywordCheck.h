//===--- AvoidMacroNamedAsCkeywordCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMACRONAMEDASCKEYWORDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMACRONAMEDASCKEYWORDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects macro names that match C language keywords.
///
/// Defining macros with names that are C keywords can cause confusion and
/// portability issues. It can make code extremely difficult to read and
/// understand, as readers would expect keyword behavior but get macro
/// expansion instead. This violates the principle of least surprise.
///
/// Related MISRA C:2025 Rule: 20.4 - A macro shall not be defined with the
/// same name as a keyword.
///
/// Example:
/// \code
///   #define if 0  // Warning: macro named as C keyword
/// \endcode
class AvoidMacroNamedAsCkeywordCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidMacroNamedAsCkeywordCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers preprocessor callbacks for macro definition handling.
  /// \param SM The source manager.
  /// \param PP The preprocessor instance.
  /// \param ModuleExpanderPP The module expander preprocessor.
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMACRONAMEDASCKEYWORDCHECK_H

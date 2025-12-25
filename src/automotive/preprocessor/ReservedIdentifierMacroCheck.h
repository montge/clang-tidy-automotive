//===--- ReservedIdentifierMacroCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_RESERVEDIDENTIFIERMACROCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_RESERVEDIDENTIFIERMACROCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-preprocessor
/// @brief Detects #define and #undef of reserved identifiers.
///
/// Reserved identifiers (those starting with underscore followed by uppercase
/// or another underscore, and standard library names) shall not be defined or
/// undefined. Modifying these can cause undefined behavior and break the
/// standard library.
///
/// @par MISRA C:2025 Rule 20.15
/// #define and #undef shall not be used on a reserved identifier or reserved
/// macro name.
/// @par Category: Required
///
/// Example:
/// @code
///   #define _RESERVED 1     // Warning: starts with underscore + uppercase
///   #define __internal 2    // Warning: starts with double underscore
///   #define errno 0         // Warning: standard library name
///   #undef NULL             // Warning: standard library name
///   #define _lowercase 1    // Compliant: single underscore + lowercase
///   #define MY_MACRO 1      // Compliant: normal identifier
/// @endcode
class ReservedIdentifierMacroCheck : public ClangTidyCheck {
public:
  ReservedIdentifierMacroCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~ReservedIdentifierMacroCheck() override = default;

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_RESERVEDIDENTIFIERMACROCHECK_H

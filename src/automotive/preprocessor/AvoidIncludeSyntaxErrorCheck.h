//===--- AvoidIncludeSyntaxErrorCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCLUDESYNTAXERRORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCLUDESYNTAXERRORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-preprocessor
/// @brief Detects non-standard #include syntax.
///
/// Implements MISRA Rule 20.3: The #include directive shall be followed by
/// either a <filename> or "filename" sequence.
///
/// @par MISRA C:2025 Rule 20.3
/// Non-standard include syntax is undefined behavior.
/// @par Category: Required
///
/// Example:
/// @code
///   #include <header.h>   // OK
///   #include "header.h"   // OK
///   #include MACRO        // Warning if MACRO doesn't expand properly
/// @endcode
class AvoidIncludeSyntaxErrorCheck : public ClangTidyCheck {
public:
  AvoidIncludeSyntaxErrorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidIncludeSyntaxErrorCheck() override = default;

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCLUDESYNTAXERRORCHECK_H

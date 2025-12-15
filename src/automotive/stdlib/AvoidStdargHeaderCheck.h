//===--- AvoidStdargHeaderCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDARGHEADERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDARGHEADERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of variable argument facilities from stdarg.h header.
///
/// Variable argument functions (va_list, va_start, va_arg, va_end) bypass
/// type safety and make it impossible for the compiler to verify correct
/// argument types and counts. This can lead to undefined behavior and
/// security vulnerabilities. Fixed-argument functions are type-safe and
/// more reliable.
///
/// Related MISRA C:2025 Rule: 17.1 - The features of <stdarg.h> shall not
/// be used.
///
/// Example:
/// \code
///   #include <stdarg.h>  // Warning: stdarg.h usage
/// \endcode
class AvoidStdargHeaderCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidStdargHeaderCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidStdargHeaderCheck() override = default;

  /// Registers preprocessor callbacks for stdarg.h inclusion.
  /// \param SM The source manager.
  /// \param PP The preprocessor instance.
  /// \param ModuleExpanderPP The module expander preprocessor.
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDARGHEADERCHECK_H

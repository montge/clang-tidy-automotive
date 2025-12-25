//===--- AvoidOffsetofCheck.h - clang-tidy ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOFFSETOFCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOFFSETOFCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects use of the offsetof macro.
///
/// The offsetof macro shall not be used in C++ code. It is a C-style
/// mechanism that relies on implementation-defined behavior and can lead
/// to undefined behavior when used with non-standard-layout types.
///
/// Related MISRA C++:2023 Rule 21.2.4 - The macro offsetof shall not be used.
///
/// Example:
/// \code
///   #include <cstddef>
///   struct S { int a; int b; };
///   size_t offset = offsetof(S, b);  // Warning: offsetof shall not be used
/// \endcode
class AvoidOffsetofCheck : public ClangTidyCheck {
public:
  AvoidOffsetofCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOFFSETOFCHECK_H

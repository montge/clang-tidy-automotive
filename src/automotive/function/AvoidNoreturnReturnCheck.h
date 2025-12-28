//===--- AvoidNoreturnReturnCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNORETURNRETURNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNORETURNRETURNCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects return statements in _Noreturn functions.
///
/// MISRA C:2025 Rule 17.9 (Mandatory):
/// A noreturn function shall not return to its caller.
///
/// Functions declared with _Noreturn must not contain return statements
/// that would return control to the caller.
///
/// Example:
/// \code
///   _Noreturn void fatal_error(void) {
///     cleanup();
///     return;  // Warning: noreturn function shall not return
///   }
/// \endcode
class AvoidNoreturnReturnCheck : public ClangTidyCheck {
public:
  AvoidNoreturnReturnCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNORETURNRETURNCHECK_H

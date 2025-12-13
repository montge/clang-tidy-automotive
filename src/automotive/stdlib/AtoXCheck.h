//===--- AtoXCheck.h - clang-tidy ------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ATOXCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ATOXCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of atoi(), atof(), and atol() string conversion functions.
///
/// The atoi family of functions has undefined behavior on conversion errors,
/// provides no error reporting mechanism, and cannot distinguish between
/// a valid result of 0 and a conversion failure. The strtol/strtod family
/// should be used instead as they provide proper error handling.
///
/// Related MISRA C:2025 Rule: 21.7 - The atof, atoi, atol and atoll functions
/// of <stdlib.h> shall not be used.
///
/// Example:
/// \code
///   int x = atoi("123");    // Warning: atoi() usage
///   double y = atof("1.5"); // Warning: atof() usage
/// \endcode
class AtoXCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AtoXCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for atoi/atof/atol calls.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched conversion function calls and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ATOXCHECK_H

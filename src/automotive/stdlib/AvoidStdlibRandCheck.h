//===--- AvoidStdlibRandCheck.h - clang-tidy ----------.---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBRANDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBRANDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of rand() and srand() functions from stdlib.h.
///
/// The rand() function produces pseudo-random numbers with implementation-
/// defined quality and is not suitable for security-critical applications.
/// The sequence is deterministic and predictable, making it inappropriate
/// for cryptographic purposes or safety-critical random number generation.
///
/// Related MISRA C:2025 Rule: 21.24 - The random number generator functions
/// of <stdlib.h> shall not be used.
///
/// Example:
/// \code
///   int x = rand();  // Warning: rand() usage
///   srand(time(0));  // Warning: srand() usage
/// \endcode
class AvoidStdlibRandCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidStdlibRandCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for rand/srand calls.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched rand/srand calls and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBRANDCHECK_H

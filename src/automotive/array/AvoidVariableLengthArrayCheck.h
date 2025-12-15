//===--- AvoidVariableLengthArrayCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDVARIABLELENGTHARRAYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDVARIABLELENGTHARRAYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of variable length arrays (VLAs).
///
/// Variable length arrays have runtime-determined size, which can lead to
/// stack overflow if the size is large or unbounded. VLAs also make static
/// analysis more difficult and are optional in C11. This check flags all
/// VLA declarations.
///
/// Related MISRA C:2025 Rule: 18.8 - Variable-length array types shall not
/// be used.
///
/// Example:
/// \code
///   void func(int n) {
///     int arr[n];  // Warning: variable length array detected
///   }
/// \endcode
class AvoidVariableLengthArrayCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidVariableLengthArrayCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidVariableLengthArrayCheck() override = default;

  /// Registers AST matchers for variable length array declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched VLA declarations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDVARIABLELENGTHARRAYCHECK_H

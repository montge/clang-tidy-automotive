//===--- AvoidPointerToVLACheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTOVLACHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTOVLACHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects pointers to variably-modified array types.
///
/// Pointers to variably-modified array types (VLAs) inherit the runtime-sizing
/// problems of VLAs themselves. Such pointers can lead to stack overflow,
/// complicate static analysis, and make it difficult to verify memory safety.
/// This check flags all pointer declarations that point to variably-modified
/// array types.
///
/// Related MISRA C:2025 Rule: 18.10 - Pointers to variably-modified array
/// types shall not be used.
///
/// Example:
/// \code
///   void func(int n) {
///     int (*p)[n];  // Warning: pointer to variably-modified array type
///   }
/// \endcode
class AvoidPointerToVLACheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidPointerToVLACheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidPointerToVLACheck() override = default;

  /// Registers AST matchers for pointer declarations to VLA types.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched pointer-to-VLA declarations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTOVLACHECK_H

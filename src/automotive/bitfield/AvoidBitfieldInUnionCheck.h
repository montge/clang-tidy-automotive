//===--- AvoidBitfieldInUnionCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBITFIELDINUNIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBITFIELDINUNIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects bit-field members within union declarations.
///
/// Bit-fields in unions have implementation-defined behavior and can lead to
/// portability issues. The storage allocation and padding of bit-fields within
/// unions varies across different compilers and architectures, making code
/// non-portable and difficult to reason about.
///
/// Related MISRA C:2025 Rule: 19.2 - A union shall not contain bit-field
/// members.
///
/// Example:
/// \code
///   union Data {
///     unsigned int flags : 8;  // Warning: bit-field in union
///     int value;
///   };
/// \endcode
class AvoidBitfieldInUnionCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidBitfieldInUnionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidBitfieldInUnionCheck() override = default;

  /// Registers AST matchers for bit-field members in unions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched bit-field members and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDBITFIELDINUNIONCHECK_H

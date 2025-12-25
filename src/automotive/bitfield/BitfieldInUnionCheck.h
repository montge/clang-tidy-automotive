//===--- BitfieldInUnionCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_BITFIELDINUNIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_BITFIELDINUNIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects bit-field declarations as members of union types.
///
/// According to MISRA C:2025 Rule 6.3, a bit-field shall not be declared
/// as a member of a union. This is because the storage allocation and
/// padding of bit-fields within unions varies across different compilers
/// and architectures, leading to implementation-defined behavior and
/// portability issues.
///
/// MISRA C:2025 Rule 6.3 (Required): A bit-field shall not be declared as
/// a member of a union.
///
/// Example:
/// \code
///   union Data {
///     unsigned int flags : 8;  // Warning: bit-field in union
///     int value;
///   };
/// \endcode
class BitfieldInUnionCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  BitfieldInUnionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~BitfieldInUnionCheck() override = default;

  /// Registers AST matchers for bit-field members in unions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched bit-field members and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_BITFIELDINUNIONCHECK_H

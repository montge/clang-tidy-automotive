//===--- AvoidSignedSingleBitFieldCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIGNEDSINGLEBITFIELDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIGNEDSINGLEBITFIELDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-types
/// @brief Detects single-bit bit-fields with signed types.
///
/// A signed bit-field with width of 1 can only represent values 0 and -1,
/// which is rarely the intended behavior and often leads to confusion.
/// Developers typically expect single-bit fields to represent boolean values
/// (0 or 1), which requires an unsigned type.
///
/// @par MISRA C:2025 Rule 6.2
/// Single-bit named bit-fields shall not be of a signed type.
/// @par Category: Required
///
/// Example:
/// \code
///   struct Flags {
///     signed int flag : 1;  // Warning: signed single-bit field
///   };
/// \endcode
class AvoidSignedSingleBitFieldCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidSignedSingleBitFieldCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for signed single-bit fields.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched signed single-bit fields and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSIGNEDSINGLEBITFIELDCHECK_H

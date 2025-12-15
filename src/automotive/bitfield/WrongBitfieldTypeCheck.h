//===--- WrongBitfieldTypeCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGBITFIELDTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGBITFIELDTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects bit-fields declared with inappropriate types.
///
/// Bit-fields should only be declared with appropriate integer types to ensure
/// portability and well-defined behavior. Using other types like enums or
/// non-standard integer types can lead to implementation-defined behavior and
/// portability issues across different compilers and platforms.
///
/// Related MISRA C:2025 Rule: 6.1 - Bit-fields shall only be declared with an
/// appropriate type.
///
/// Example:
/// \code
///   struct Config {
///     _Bool flag : 2;     // Warning: inappropriate type for bit-field
///     float value : 4;    // Warning: inappropriate type for bit-field
///   };
/// \endcode
class WrongBitfieldTypeCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  WrongBitfieldTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~WrongBitfieldTypeCheck() override = default;

  /// Registers AST matchers for bit-field declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched bit-fields and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGBITFIELDTYPECHECK_H

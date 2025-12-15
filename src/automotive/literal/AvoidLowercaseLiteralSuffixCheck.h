//===--- AvoidLowercaseLiteralSuffixCheck.h - clang-tidy --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLOWERCASELITERALSUFFIXCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLOWERCASELITERALSUFFIXCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects lowercase literal suffixes on numeric constants.
///
/// Lowercase 'l' can be easily confused with the digit '1', especially in
/// certain fonts. Using uppercase 'L' or 'U' for literal suffixes improves
/// readability and prevents misinterpretation of numeric literals.
///
/// Related MISRA C:2025 Rule: 7.3 - The lowercase character 'l' shall not be
/// used in a literal suffix.
///
/// Example:
/// \code
///   long x = 100l;   // Warning: lowercase 'l' suffix
///   unsigned y = 5u; // Warning: lowercase 'u' suffix
/// \endcode
class AvoidLowercaseLiteralSuffixCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidLowercaseLiteralSuffixCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidLowercaseLiteralSuffixCheck() override = default;

  /// Registers AST matchers for numeric literals.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched literals and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDLOWERCASELITERALSUFFIXCHECK_H

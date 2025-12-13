//===--- UniqueEnumValuesCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNIQUEENUMVALUESCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNIQUEENUMVALUESCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects duplicate values in enumeration constants.
///
/// Each enumerator in an enumeration should have a unique value. Duplicate
/// values can make the code confusing and error-prone, as multiple enum
/// constants evaluate to the same value, which defeats the purpose of using
/// named constants and can lead to logic errors in switch statements.
///
/// Related MISRA C:2025 Rule: 9.5 - Each enumerator within an enumeration
/// shall have a unique value.
///
/// Example:
/// \code
///   enum Status {
///     OK = 0,
///     SUCCESS = 0  // Warning: duplicate value
///   };
/// \endcode
class UniqueEnumValuesCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  UniqueEnumValuesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for enum declarations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched enumerations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNIQUEENUMVALUESCHECK_H

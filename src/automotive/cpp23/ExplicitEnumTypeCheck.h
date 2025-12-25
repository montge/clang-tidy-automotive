//===--- ExplicitEnumTypeCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_EXPLICITENUMTYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_EXPLICITENUMTYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-types
/// @brief Detects enum declarations without explicit underlying type.
///
/// Enumeration underlying type shall be explicitly specified to ensure
/// portability and predictable behavior across different platforms and
/// compilers.
///
/// @par MISRA C++:2023 Rule 10.2.1
/// The underlying type of an enumeration shall be explicitly defined.
/// @par Category: Required
///
/// Example:
/// @code
///   enum Color { Red, Green, Blue };        // Warning: no underlying type
///   enum class Status { OK, Error };        // Warning: no underlying type
///   enum Color : int { Red, Green, Blue };  // OK
///   enum class Status : uint8_t { OK };     // OK
/// @endcode
class ExplicitEnumTypeCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  ExplicitEnumTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~ExplicitEnumTypeCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for enum declarations without explicit type.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched enum declarations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_EXPLICITENUMTYPECHECK_H

//===--- NoexceptMoveCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_NOEXCEPTMOVECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_NOEXCEPTMOVECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-exceptions
/// @brief Detects move constructors and move assignment operators not marked
/// noexcept.
///
/// Move operations should be declared noexcept to enable optimizations in
/// standard library containers and algorithms. Non-noexcept move operations
/// can cause the standard library to use copy operations instead.
///
/// @par MISRA C++:2023 Rule 18.4.2
/// Move constructors and move assignment operators shall be noexcept.
/// @par Category: Required
///
/// Example:
/// @code
///   class Foo {
///     Foo(Foo&&);  // Warning: should be noexcept
///     Foo(Foo&&) noexcept;  // OK
///     Foo& operator=(Foo&&);  // Warning: should be noexcept
///     Foo& operator=(Foo&&) noexcept;  // OK
///   };
/// @endcode
class NoexceptMoveCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  NoexceptMoveCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++11 and later.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

  /// Registers AST matchers for move operations without noexcept.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched move operations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_NOEXCEPTMOVECHECK_H

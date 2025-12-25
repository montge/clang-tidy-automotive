//===--- UseOverrideCheck.h - clang-tidy -------------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USEOVERRIDECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USEOVERRIDECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-classes
/// @brief Detects virtual member functions that override a base class function
/// but do not have the 'override' specifier.
///
/// The 'override' specifier makes the intent explicit that a function is meant
/// to override a base class virtual function. It helps catch errors at compile
/// time if the function signature doesn't match any base class virtual
/// function.
///
/// @par MISRA C++:2023 Rule 13.3.2
/// An override function shall be declared with the override specifier.
/// @par Category: Required
///
/// Example:
/// @code
///   class Base {
///   public:
///     virtual void foo();
///   };
///
///   class Derived : public Base {
///   public:
///     void foo();  // Warning: missing override specifier
///     void bar() override;  // OK
///   };
/// @endcode
class UseOverrideCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  UseOverrideCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~UseOverrideCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++11 and later.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

  /// Registers AST matchers for methods that override base class methods.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched methods and checks for override specifier.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USEOVERRIDECHECK_H

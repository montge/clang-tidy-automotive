//===--- AvoidVirtualFunctionDefaultArgCheck.h - clang-tidy ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALFUNCTIONDEFAULTARGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALFUNCTIONDEFAULTARGCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-functions
/// @brief Detects virtual functions with default arguments.
///
/// Default arguments in virtual functions can cause unexpected behavior because
/// the default value is determined by the static type of the object, not the
/// dynamic type. This can lead to different default values being used depending
/// on how the function is called, even though the same virtual function
/// implementation is executed.
///
/// @par MISRA C++:2023 Rule 12.4.1
/// Virtual functions shall not have default arguments.
/// @par Category: Required
///
/// Example:
/// @code
///   class Base {
///   public:
///     virtual void foo(int x = 0);  // Warning: virtual function with default
///     arg
///   };
///
///   class Derived : public Base {
///   public:
///     void foo(int x = 1) override;  // Warning: overriding with different
///     default
///   };
/// @endcode
class AvoidVirtualFunctionDefaultArgCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidVirtualFunctionDefaultArgCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidVirtualFunctionDefaultArgCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for virtual functions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched virtual functions and checks for default arguments.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALFUNCTIONDEFAULTARGCHECK_H

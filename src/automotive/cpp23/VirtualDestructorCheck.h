//===--- VirtualDestructorCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_VIRTUALDESTRUCTORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_VIRTUALDESTRUCTORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-classes
/// @brief Detects base classes with virtual functions but non-virtual
/// destructors.
///
/// A class with virtual functions that is intended to be used polymorphically
/// should have a virtual destructor. Without a virtual destructor, deleting a
/// derived class object through a base class pointer results in undefined
/// behavior as the derived class destructor will not be called.
///
/// @par MISRA C++:2023 Rule 15.7.1
/// A class which has virtual functions shall have a virtual destructor.
/// @par Category: Required
///
/// Example:
/// @code
///   class Base {
///   public:
///     virtual void foo();
///     ~Base() { }  // Warning: non-virtual destructor in class with virtual
///     functions
///   };
/// @endcode
class VirtualDestructorCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  VirtualDestructorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~VirtualDestructorCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for classes with virtual functions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched classes and checks destructor virtuality.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_VIRTUALDESTRUCTORCHECK_H

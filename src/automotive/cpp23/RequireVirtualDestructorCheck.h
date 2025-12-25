//===--- RequireVirtualDestructorCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_REQUIREVIRTUALDESTRUCTORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_REQUIREVIRTUALDESTRUCTORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-classes
/// @brief Detects base classes with public non-virtual destructors or
/// non-protected non-virtual destructors.
///
/// A base class destructor shall be either:
/// - Public and virtual, or
/// - Protected and non-virtual
///
/// This ensures that derived class objects can be safely deleted through base
/// class pointers when the destructor is public and virtual, or prevents
/// deletion through base class pointers when the destructor is protected.
///
/// @par MISRA C++:2023 Rule 12.2.1
/// A base class destructor shall be public and virtual, or protected and
/// non-virtual.
/// @par Category: Required
///
/// Example violation:
/// @code
///   class Base {
///   public:
///     virtual void foo();
///     ~Base() { }  // Warning: public non-virtual destructor
///   };
///
///   class Derived : public Base {
///   public:
///     ~Derived() { }
///   };
/// @endcode
///
/// Compliant examples:
/// @code
///   // Option 1: Public virtual destructor
///   class Base1 {
///   public:
///     virtual void foo();
///     virtual ~Base1() { }  // OK: public and virtual
///   };
///
///   // Option 2: Protected non-virtual destructor
///   class Base2 {
///   public:
///     virtual void foo();
///   protected:
///     ~Base2() { }  // OK: protected and non-virtual
///   };
/// @endcode
class RequireVirtualDestructorCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  RequireVirtualDestructorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~RequireVirtualDestructorCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for classes with destructors.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched classes and checks destructor access and virtuality.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_REQUIREVIRTUALDESTRUCTORCHECK_H

//===--- AvoidVirtualBaseClassCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALBASECLASSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALBASECLASSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-classes
/// @brief Detects classes that use virtual inheritance.
///
/// Virtual base classes add complexity and overhead to the inheritance
/// hierarchy. They should be avoided unless necessary for resolving diamond
/// inheritance problems or other specific design requirements.
///
/// @par MISRA C++:2023 Rule 15.2
/// Virtual base classes should be avoided unless necessary.
/// @par Category: Advisory
///
/// Example violation:
/// @code
///   class Base {
///   public:
///     int x;
///   };
///
///   class Derived : virtual public Base {  // Warning: virtual inheritance
///   public:
///     int y;
///   };
/// @endcode
///
/// Compliant:
/// @code
///   class Base {
///   public:
///     int x;
///   };
///
///   class Derived : public Base {  // OK: non-virtual inheritance
///   public:
///     int y;
///   };
/// @endcode
class AvoidVirtualBaseClassCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidVirtualBaseClassCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidVirtualBaseClassCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for classes with virtual base specifiers.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched classes and checks for virtual inheritance.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALBASECLASSCHECK_H

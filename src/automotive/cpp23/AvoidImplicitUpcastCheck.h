//===--- AvoidImplicitUpcastCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITUPCASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITUPCASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-expressions
/// @brief Detects implicit upcasting from derived class pointers/references to
/// base class pointers/references.
///
/// This check detects implicit cast operations that convert pointers or
/// references from a derived class type to a base class type. Such implicit
/// upcasts can lead to object slicing when used improperly and should be made
/// explicit for clarity and safety.
///
/// @par MISRA C++:2023 Rule 19.2
/// A reference or pointer to a derived class shall not be implicitly cast to a
/// reference or pointer to a base class.
/// @par Category: Required
///
/// Example violations:
/// @code
///   class Base {
///   public:
///     int x;
///     virtual void foo() {}
///   };
///
///   class Derived : public Base {
///   public:
///     int y;
///     void foo() override {}
///   };
///
///   void takeBase(Base* b) {}
///   void takeBaseRef(Base& b) {}
///
///   void test() {
///     Derived d;
///     Derived* pd = &d;
///
///     Base* b = pd;           // Warning: implicit upcast pointer
///     Base& br = d;           // Warning: implicit upcast reference
///     takeBase(pd);           // Warning: implicit upcast in argument
///     takeBaseRef(d);         // Warning: implicit upcast in argument
///   }
/// @endcode
///
/// Compliant examples:
/// @code
///   void test() {
///     Derived d;
///     Derived* pd = &d;
///
///     Base* b = static_cast<Base*>(pd);      // OK: explicit cast
///     Base& br = static_cast<Base&>(d);      // OK: explicit cast
///     takeBase(static_cast<Base*>(pd));      // OK: explicit cast
///     takeBaseRef(static_cast<Base&>(d));    // OK: explicit cast
///   }
/// @endcode
class AvoidImplicitUpcastCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidImplicitUpcastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidImplicitUpcastCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for implicit upcast operations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched implicit upcast operations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDIMPLICITUPCASTCHECK_H

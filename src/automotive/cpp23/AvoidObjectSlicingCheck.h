//===--- AvoidObjectSlicingCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOBJECTSLICINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOBJECTSLICINGCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-classes
/// @brief Detects object slicing when a derived class object is assigned to a
/// base class object by value.
///
/// Object slicing occurs when a derived class object is copied or assigned to
/// a base class object by value. This causes the derived class-specific data
/// members and virtual function overrides to be "sliced off", leaving only the
/// base class portion. This often leads to incorrect behavior and is generally
/// unintended.
///
/// This check detects object slicing in several scenarios:
/// - Assignment of derived class to base class by value
/// - Passing derived class to function taking base class by value
/// - Returning derived class from function returning base class by value
/// - Copy construction from derived to base
///
/// @par MISRA C++:2023 Rule 11.0.1
/// Object slicing shall not occur.
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
///   void takeByValue(Base b) {}  // Function parameter by value
///
///   Base returnByValue(Derived d) {
///     return d;  // Warning: object slicing on return
///   }
///
///   void test() {
///     Derived d;
///     Base b = d;        // Warning: object slicing on assignment
///     takeByValue(d);    // Warning: object slicing on function call
///     Base b2(d);        // Warning: object slicing on construction
///   }
/// @endcode
///
/// Compliant examples:
/// @code
///   void takeByReference(const Base& b) {}  // OK: reference
///   void takeByPointer(Base* b) {}           // OK: pointer
///
///   void test() {
///     Derived d;
///     Base& b = d;           // OK: reference
///     Base* b2 = &d;         // OK: pointer
///     takeByReference(d);    // OK: passed by reference
///     takeByPointer(&d);     // OK: passed by pointer
///   }
/// @endcode
class AvoidObjectSlicingCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidObjectSlicingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidObjectSlicingCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for object slicing scenarios.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched slicing operations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOBJECTSLICINGCHECK_H

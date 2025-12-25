//===--- AvoidVirtualCallInCtorDtorCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALCALLINCTORDTORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALCALLINCTORDTORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-classes
/// @brief Detects virtual function calls during construction or destruction.
///
/// Calling virtual functions from constructors or destructors is problematic
/// because the dynamic dispatch behavior may not work as expected. During
/// construction and destruction, the object is in an intermediate state where
/// virtual function calls may resolve to the base class implementation rather
/// than the derived class implementation.
///
/// @par MISRA C++:2023 Rule 17.8
/// Virtual functions shall not be called during construction or destruction.
/// @par Category: Required
/// @par Decidability: Partially decidable (detects direct calls)
///
/// Example violation:
/// @code
///   class Base {
///   public:
///     Base() {
///       init();  // Warning: calls virtual function in constructor
///     }
///     virtual ~Base() {
///       cleanup();  // Warning: calls virtual function in destructor
///     }
///     virtual void init();
///     virtual void cleanup();
///   };
/// @endcode
///
/// Compliant example:
/// @code
///   class Base {
///   public:
///     Base() {
///       initImpl();  // OK: calls non-virtual function
///     }
///     virtual ~Base() {
///       cleanupImpl();  // OK: calls non-virtual function
///     }
///     virtual void init();
///   private:
///     void initImpl();  // Non-virtual implementation
///     void cleanupImpl();  // Non-virtual implementation
///   };
/// @endcode
class AvoidVirtualCallInCtorDtorCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidVirtualCallInCtorDtorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidVirtualCallInCtorDtorCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for constructors and destructors.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched constructors/destructors and checks for virtual calls.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVIRTUALCALLINCTORDTORCHECK_H

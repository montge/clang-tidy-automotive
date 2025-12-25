//===--- UseNodiscardCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USENODISCARDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USENODISCARDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Enforces the use of [[nodiscard]] attribute on functions returning values
/// that should not be ignored.
///
/// MISRA C++:2023 Rule 10.6.1: Functions that return values that should not be
/// ignored shall be declared with the [[nodiscard]] attribute (Advisory).
///
/// This check detects functions that:
/// - Return non-void values
/// - Are public member functions or free functions
/// - Do NOT have [[nodiscard]] attribute
/// - Return types that indicate error conditions (bool, error codes, pointers,
///   optionals)
///
/// Functions that are NOT flagged:
/// - Functions already marked [[nodiscard]]
/// - Constructors, destructors
/// - Operators (operator=, etc.)
/// - Functions in system headers
/// - Private/protected member functions (configurable)
///
/// Example:
/// @code
///   // Non-compliant:
///   bool validate();           // violation
///   int* allocate();          // violation
///   std::optional<T> find();  // violation
///
///   // Compliant:
///   [[nodiscard]] bool validate();
///   [[nodiscard]] int* allocate();
///   [[nodiscard]] std::optional<T> find();
///   void process();  // OK - returns void
/// @endcode
///
/// Options:
///   CheckPrivateFunctions (default: false) - Also check private/protected
///                                            member functions
class UseNodiscardCheck : public ClangTidyCheck {
public:
  UseNodiscardCheck(StringRef Name, ClangTidyContext *Context);

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus17; // [[nodiscard]] requires C++17
  }

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool CheckPrivateFunctions;

  bool shouldCheckReturnType(QualType ReturnType) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_USENODISCARDCHECK_H

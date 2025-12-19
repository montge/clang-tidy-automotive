//===--- RuleOfFiveCheck.h - clang-tidy -------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_RULEOFFIVECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_RULEOFFIVECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-classes
/// @brief Enforces the Rule of Five for classes with custom resource
/// management.
///
/// If a class defines any of the following special member functions, it should
/// define all five: destructor, copy constructor, copy assignment operator,
/// move constructor, and move assignment operator.
///
/// @par MISRA C++:2023 Rule 15.0.1
/// If a class has a user-declared destructor, copy/move constructor, or
/// copy/move assignment operator, then all five should be declared.
/// @par Category: Required
///
/// Example:
/// @code
///   class Resource {
///     int* ptr;
///   public:
///     ~Resource() { delete ptr; }  // Warning: defines destructor but not
///     others
///   };
/// @endcode
class RuleOfFiveCheck : public ClangTidyCheck {
public:
  RuleOfFiveCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~RuleOfFiveCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11; // Rule of Five requires C++11 for move
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_RULEOFFIVECHECK_H

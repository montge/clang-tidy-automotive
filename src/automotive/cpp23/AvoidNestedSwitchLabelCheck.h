//===--- AvoidNestedSwitchLabelCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNESTEDSWITCHLABELCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNESTEDSWITCHLABELCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-statement
/// @brief Detects switch labels that are not in the outermost compound
/// statement.
///
/// Switch case labels should only appear at the outermost level of the switch
/// body, not nested inside other compound statements, loops, or conditionals.
/// This improves code clarity and prevents confusing control flow.
///
/// @par MISRA C++:2023 Rule 9.5 (Required)
/// A switch label shall only be used in the outermost compound statement
/// of a switch body.
///
/// Example:
/// @code
///   switch (x) {
///     case 1:  // Compliant
///       break;
///     case 2: {  // Warning: nested case label
///       if (y) {
///         case 3:  // Warning: case inside nested block
///           break;
///       }
///     }
///   }
/// @endcode
class AvoidNestedSwitchLabelCheck : public ClangTidyCheck {
public:
  AvoidNestedSwitchLabelCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNESTEDSWITCHLABELCHECK_H

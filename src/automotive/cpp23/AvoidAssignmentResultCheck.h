//===--- AvoidAssignmentResultCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDASSIGNMENTRESULTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDASSIGNMENTRESULTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-expression
/// @brief Detects when the result of an assignment is used.
///
/// The result of an assignment shall not be used, except in chained
/// assignments where the assignment is the direct operand of another
/// assignment operator.
///
/// @par MISRA C++:2023 Rule 7.4 (Required)
/// The result of an assignment shall not be used.
///
/// Example:
/// @code
///   // Compliant:
///   x = getValue();
///   if (x != 0) { }
///   a = b = c;  // Chained assignment is allowed
///
///   // Non-compliant - assignment result used in condition:
///   if (x = getValue()) { }
///
///   // Non-compliant - assignment result used in expression:
///   y = (x = 5) + 1;
///
///   // Non-compliant - assignment result returned:
///   return (x = 5);
/// @endcode
class AvoidAssignmentResultCheck : public ClangTidyCheck {
public:
  AvoidAssignmentResultCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDASSIGNMENTRESULTCHECK_H

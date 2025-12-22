//===--- AvoidNullLiteralAssignmentCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNULLLITERALASSIGNMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNULLLITERALASSIGNMENTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-types
/// @brief Detects assignment of literal values to pointers instead of nullptr.
///
/// Using `0`, `0L`, or similar integer literals for null pointer values
/// is deprecated in C++11 and later. The nullptr literal should be used
/// instead for type safety and clarity.
///
/// @par MISRA C++:2023 Rule 7.11 (Required)
/// A literal value shall not be assigned to a null pointer constant.
///
/// Example:
/// @code
///   // Compliant:
///   int* p = nullptr;
///   p = nullptr;
///
///   // Non-compliant - using literal 0:
///   int* q = 0;
///   q = 0;
/// @endcode
class AvoidNullLiteralAssignmentCheck : public ClangTidyCheck {
public:
  AvoidNullLiteralAssignmentCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNULLLITERALASSIGNMENTCHECK_H

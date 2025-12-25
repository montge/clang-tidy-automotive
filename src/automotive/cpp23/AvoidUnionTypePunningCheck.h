//===--- AvoidUnionTypePunningCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNIONTYPEPUNNINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNIONTYPEPUNNINGCHECK_H

#include "../../ClangTidyCheck.h"
#include <map>
#include <string>

namespace clang::tidy::automotive::cpp23 {

/// Detects type-punning through union members where different types are
/// written and read.
///
/// Type-punning through unions leads to undefined behavior when a union
/// is written through one member and read through another member of a
/// different type (except for specific cases allowed by the standard).
/// This violates type safety and can lead to unpredictable behavior.
///
/// Related MISRA C++:2023 Rule 19.1 (Required)
///
/// Example:
/// \code
///   union Data {
///     int i;
///     float f;
///   };
///
///   void foo() {
///     Data d;
///     d.i = 42;      // Write as int
///     float x = d.f; // Warning: reading float after writing int
///   }
/// \endcode
class AvoidUnionTypePunningCheck : public ClangTidyCheck {
public:
  AvoidUnionTypePunningCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  // Track which member of a union was last written to
  // Map: union variable name -> last written field name
  std::map<std::string, std::string> LastWrittenMember;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNIONTYPEPUNNINGCHECK_H

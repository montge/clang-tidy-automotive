//===--- AvoidNonArrayPointerArithCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNONARRAYPOINTERARITHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNONARRAYPOINTERARITHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects pointer arithmetic applied to pointers to non-array objects.
///
/// MISRA C++:2023 Rule 8.7 (Partial): Pointer arithmetic shall not be applied
/// to a pointer to object of non-array type.
///
/// This check detects obvious violations:
/// - Pointer arithmetic on address-of expressions of non-array variables: &x +
/// 1
/// - Pointer arithmetic on new T (single object allocation)
/// - Increment/decrement of pointers that are clearly not array pointers
class AvoidNonArrayPointerArithCheck : public ClangTidyCheck {
public:
  AvoidNonArrayPointerArithCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDNONARRAYPOINTERARITHCHECK_H

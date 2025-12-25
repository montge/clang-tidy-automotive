//===--- CheckFilePointerValidityCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_CHECKFILEPOINTERVALIDITYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_CHECKFILEPOINTERVALIDITYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects invalid FILE* pointer usage.
///
/// MISRA C++:2023 Rule 21.2 (Partial): A pointer to a file shall have a
/// valid value when used.
///
/// This check detects:
/// - Using FILE* after fclose() without reassignment
/// - Dereferencing potentially NULL FILE* from fopen() without checking
/// - Using uninitialized FILE* pointers
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/automotive/cpp23-adv-21.2.html
class CheckFilePointerValidityCheck : public ClangTidyCheck {
public:
  CheckFilePointerValidityCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_CHECKFILEPOINTERVALIDITYCHECK_H

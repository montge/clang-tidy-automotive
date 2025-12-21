//===--- AvoidNoreturnNonVoidCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNORETURNNONVOIDCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNORETURNNONVOIDCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects _Noreturn functions with non-void return type.
///
/// A function declared with _Noreturn specifier shall have void return type
/// since it never returns a value to the caller.
///
/// Related MISRA C:2025 Rule 17.10
///
/// Example:
/// \code
///   _Noreturn int foo(void);  // Warning: non-void return type
///   _Noreturn void bar(void); // OK
/// \endcode
class AvoidNoreturnNonVoidCheck : public ClangTidyCheck {
public:
  AvoidNoreturnNonVoidCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNORETURNNONVOIDCHECK_H

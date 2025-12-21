//===--- AvoidStringLiteralToNonConstCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTRINGLITERALTONONCONST_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTRINGLITERALTONONCONST_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects string literals assigned to non-const-qualified char pointers.
///
/// String literals should only be assigned to pointers to const-qualified
/// char types to prevent accidental modification of read-only data.
///
/// Related MISRA C:2025 Rule 7.4
///
/// Example:
/// \code
///   char *p = "hello";        // Warning: string literal to non-const pointer
///   const char *q = "hello";  // OK: pointer to const char
/// \endcode
class AvoidStringLiteralToNonConstCheck : public ClangTidyCheck {
public:
  AvoidStringLiteralToNonConstCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTRINGLITERALTONONCONST_H

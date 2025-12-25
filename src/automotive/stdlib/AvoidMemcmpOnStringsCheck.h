//===--- AvoidMemcmpOnStringsCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMEMCMPONSTRINGSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMEMCMPONSTRINGSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of memcmp to compare null-terminated strings.
///
/// MISRA C:2025 Rule 21.14 prohibits using memcmp() to compare null-terminated
/// strings because it may compare beyond the null terminator, leading to
/// undefined behavior or incorrect results.
///
/// Use strcmp() or strncmp() instead for string comparisons.
///
/// Example:
/// \code
///   char str1[] = "hello";
///   char str2[] = "world";
///   memcmp(str1, str2, 5);  // Warning: use strcmp for strings
/// \endcode
class AvoidMemcmpOnStringsCheck : public ClangTidyCheck {
public:
  AvoidMemcmpOnStringsCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMEMCMPONSTRINGSCHECK_H

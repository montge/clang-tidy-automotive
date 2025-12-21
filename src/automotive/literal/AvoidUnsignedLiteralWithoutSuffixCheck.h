//===--- AvoidUnsignedLiteralWithoutSuffixCheck.h - clang-tidy --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNSIGNEDLITERALWITHOUTSUFFIXCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNSIGNEDLITERALWITHOUTSUFFIXCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects integer constants with unsigned type that lack a 'u' or 'U' suffix.
///
/// Integer constants that are represented in an unsigned type should have an
/// explicit unsigned suffix to make the signedness clear and avoid confusion
/// about the type of the constant.
///
/// Related MISRA C:2025 Rule 7.2
///
/// Example:
/// \code
///   unsigned int x = 0x8000;  // Warning: unsigned constant without suffix
///   unsigned int y = 0x8000U; // OK: explicit unsigned suffix
/// \endcode
class AvoidUnsignedLiteralWithoutSuffixCheck : public ClangTidyCheck {
public:
  AvoidUnsignedLiteralWithoutSuffixCheck(StringRef Name,
                                         ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDUNSIGNEDLITERALWITHOUTSUFFIXCHECK_H

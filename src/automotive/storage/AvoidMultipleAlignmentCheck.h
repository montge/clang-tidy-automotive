//===--- AvoidMultipleAlignmentCheck.h - clang-tidy --------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMULTIPLEALIGNMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMULTIPLEALIGNMENTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-storage
/// @brief Detects multiple alignment specifiers on a single declaration.
///
/// At most one explicit alignment specifier should appear in an object
/// declaration to avoid confusion and potential undefined behavior.
///
/// @par MISRA C:2025 Rule 8.17 (Advisory)
/// At most one explicit alignment specifier should appear in an object
/// declaration.
///
/// Example:
/// @code
///   // Compliant:
///   _Alignas(16) int aligned_int;
///
///   // Non-compliant - multiple alignment specifiers:
///   _Alignas(8) _Alignas(16) int multi_aligned;
/// @endcode
class AvoidMultipleAlignmentCheck : public ClangTidyCheck {
public:
  AvoidMultipleAlignmentCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDMULTIPLEALIGNMENTCHECK_H

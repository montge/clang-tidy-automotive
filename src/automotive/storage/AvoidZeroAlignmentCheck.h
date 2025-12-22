//===--- AvoidZeroAlignmentCheck.h - clang-tidy ------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDZEROALIGNMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDZEROALIGNMENTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-storage
/// @brief Detects alignment specifiers with zero value.
///
/// Using zero as an alignment value in _Alignas or alignas specifiers
/// results in undefined behavior and should be avoided.
///
/// @par MISRA C:2025 Rule 8.16 (Required)
/// An alignment specification shall not specify an alignment of zero.
///
/// Example:
/// @code
///   // Compliant:
///   _Alignas(16) int aligned_int;
///   alignas(8) double aligned_double;
///
///   // Non-compliant - zero alignment:
///   _Alignas(0) int bad_int;
///   alignas(0) double bad_double;
/// @endcode
class AvoidZeroAlignmentCheck : public ClangTidyCheck {
public:
  AvoidZeroAlignmentCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDZEROALIGNMENTCHECK_H

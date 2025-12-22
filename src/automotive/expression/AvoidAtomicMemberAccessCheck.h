//===--- AvoidAtomicMemberAccessCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDATOMICMEMBERACCESSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDATOMICMEMBERACCESSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects direct member access on atomic struct/union objects.
///
/// Structure and union members of atomic objects shall not be directly
/// accessed. Accessing individual members of an atomic composite type
/// breaks atomicity guarantees.
///
/// @par MISRA C:2025 Rule 12.6 (Required)
/// Structure and union members of atomic objects shall not be directly
/// accessed.
///
/// Example:
/// @code
///   _Atomic struct { int x; int y; } atomic_point;
///
///   // Compliant - use atomic_load/atomic_store for entire object
///   struct { int x; int y; } temp;
///   temp = atomic_load(&atomic_point);
///
///   // Non-compliant - direct member access:
///   int val = atomic_point.x;  // Breaks atomicity
///   atomic_point.y = 5;        // Breaks atomicity
/// @endcode
class AvoidAtomicMemberAccessCheck : public ClangTidyCheck {
public:
  AvoidAtomicMemberAccessCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDATOMICMEMBERACCESSCHECK_H

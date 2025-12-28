//===--- AvoidOverlappingAssignmentCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOVERLAPPINGASSIGNMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOVERLAPPINGASSIGNMENTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects assignments where objects are assigned to overlapping objects.
///
/// MISRA C:2025 Rule 19.1 (Mandatory):
/// Objects shall not be assigned to an overlapping object.
///
/// Assigning an object to itself or to an overlapping object leads to
/// undefined behavior. This check detects obvious cases such as:
/// - Self-assignment (x = x)
/// - Assignments through aliased pointers
/// - memcpy/memmove with overlapping regions (basic detection)
///
/// Example:
/// \code
///   int x;
///   x = x;  // Warning: self-assignment
///
///   int *p1 = &x, *p2 = &x;
///   *p1 = *p2;  // Warning: potentially overlapping
/// \endcode
class AvoidOverlappingAssignmentCheck : public ClangTidyCheck {
public:
  AvoidOverlappingAssignmentCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool mayOverlap(const Expr *LHS, const Expr *RHS, ASTContext &Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDOVERLAPPINGASSIGNMENTCHECK_H

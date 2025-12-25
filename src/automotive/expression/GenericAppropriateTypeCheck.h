//===--- GenericAppropriateTypeCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICAPPROPRIATETYPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICAPPROPRIATETYPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// MISRA C:2025 Rule 23.4 - A generic association shall list an appropriate
/// type.
///
/// The types listed in generic associations should be object types that
/// could potentially match the controlling expression. Inappropriate types
/// include:
/// - void type (cannot be the type of an expression)
/// - Function types (expressions have pointer-to-function, not function type)
/// - Incomplete types
/// - Array types with unknown bound
///
/// Example:
/// @code
///   _Generic(x,
///     void: 0,      // Non-compliant: void is not an object type
///     int: 1,       // Compliant
///     int[]: 2)     // Non-compliant: incomplete array type
/// @endcode
class GenericAppropriateTypeCheck : public ClangTidyCheck {
public:
  GenericAppropriateTypeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICAPPROPRIATETYPECHECK_H

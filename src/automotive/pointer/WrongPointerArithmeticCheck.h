//===--- WrongPointerArithmeticCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGPOINTERARITHMETICCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGPOINTERARITHMETICCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects pointer arithmetic applied to pointers that do not address
/// array elements.
///
/// MISRA C:2025 Rule 18.10 (Mandatory):
/// Pointer arithmetic shall only be applied to pointers that address
/// array elements.
///
/// This check identifies pointer arithmetic on:
/// - Single objects (not arrays)
/// - Pointers obtained from malloc/allocation
/// - Pointers that don't clearly reference array elements
///
/// Example:
/// \code
///   int x;
///   int *p = &x;
///   p++;  // Warning: not an array element
///
///   int arr[10];
///   int *q = arr;
///   q++;  // OK: array element
/// \endcode
class WrongPointerArithmeticCheck : public ClangTidyCheck {
public:
  WrongPointerArithmeticCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isArrayElementPointer(const Expr *E, ASTContext &Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGPOINTERARITHMETICCHECK_H

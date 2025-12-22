//===--- MissingPriorDeclarationCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGPRIORDECLARATIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGPRIORDECLARATIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects external definitions without a prior compatible declaration.
///
/// MISRA C:2012 Rule 8.4: A compatible declaration shall be visible when
/// an object or function with external linkage is defined.
///
/// This helps ensure that declarations in headers match definitions in
/// source files, preventing ODR violations and interface mismatches.
///
/// Example:
/// \code
///   // In source.c without prior declaration in header:
///   int global_var = 42;  // Warning: no prior declaration
///   void helper(void) {}  // Warning: no prior declaration
///
///   // Correct usage:
///   // header.h: extern int global_var;
///   // source.c: #include "header.h"
///   //           int global_var = 42;  // OK
/// \endcode
class MissingPriorDeclarationCheck : public ClangTidyCheck {
public:
  MissingPriorDeclarationCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGPRIORDECLARATIONCHECK_H

//===--- AvoidNonConstPointerParamCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONCONSTPOINTERPARAMCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONCONSTPOINTERPARAMCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects pointer parameters that should be declared const (MISRA C:2025 Rule 8.13).
///
/// If a pointer parameter is not modified within a function, it should be
/// declared as a pointer to const. This makes the code more readable and helps
/// prevent unintentional modifications.
///
/// \code
/// // Non-compliant
/// void func(int *p) {
///   int x = *p;  // p is not modified, should be const
/// }
///
/// // Compliant
/// void func(const int *p) {
///   int x = *p;
/// }
/// \endcode
class AvoidNonConstPointerParamCheck : public ClangTidyCheck {
public:
  AvoidNonConstPointerParamCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check if a parameter is modified in the function body
  bool isParameterModified(const ParmVarDecl *Param,
                           const FunctionDecl *Func) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNONCONSTPOINTERPARAMCHECK_H

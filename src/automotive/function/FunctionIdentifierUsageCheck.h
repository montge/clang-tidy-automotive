//===--- FunctionIdentifierUsageCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FUNCTIONIDENTIFIERUSAGECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FUNCTIONIDENTIFIERUSAGECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-function
/// @brief Detects function identifiers used without & or parenthesized parameter list.
///
/// A function identifier should only be used with either the address-of operator (&)
/// or followed by a parenthesized parameter list (function call). Using a function
/// identifier in other contexts (e.g., direct assignment to a pointer) is discouraged
/// as it can reduce code clarity.
///
/// @par MISRA C:2025 Rule 17.12
/// A function identifier should only be used with & or parenthesized parameter list.
/// @par Category: Advisory
///
/// Example:
/// @code
///   void func(void);
///   void (*ptr1)(void) = func;    // Warning: function without & or ()
///   void (*ptr2)(void) = &func;   // Compliant: explicit address-of
///   func();                       // Compliant: function call with ()
/// @endcode
class FunctionIdentifierUsageCheck : public ClangTidyCheck {
public:
  FunctionIdentifierUsageCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~FunctionIdentifierUsageCheck() override = default;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_FUNCTIONIDENTIFIERUSAGECHECK_H

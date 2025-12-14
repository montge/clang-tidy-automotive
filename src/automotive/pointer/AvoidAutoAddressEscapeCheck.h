//===--- AvoidAutoAddressEscapeCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAUTOADDRESSESCAPECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAUTOADDRESSESCAPECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects address of automatic objects escaping their scope.
///
/// MISRA C:2025 Rule 18.6 requires that the address of an object with
/// automatic storage duration shall not be copied to another object that
/// persists after the first object has ceased to exist.
///
/// This check detects common cases where an address of a local variable:
/// - Is returned from a function
/// - Is assigned to a global or static pointer
/// - Is stored in a struct/array that outlives the local
///
/// Example of non-compliant code:
/// \code
///   int *global_ptr;
///   int *get_ptr(void) {
///     int local = 42;
///     return &local;      // Address escapes via return
///   }
///   void store_ptr(void) {
///     int local = 42;
///     global_ptr = &local; // Address escapes to global
///   }
/// \endcode
class AvoidAutoAddressEscapeCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  AvoidAutoAddressEscapeCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for address-of expressions.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched expressions and emits diagnostics.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Checks if a VarDecl has automatic storage duration.
  bool hasAutomaticStorage(const VarDecl *VD) const;

  /// Checks if a VarDecl is a global or static variable.
  bool isGlobalOrStatic(const VarDecl *VD) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAUTOADDRESSESCAPECHECK_H

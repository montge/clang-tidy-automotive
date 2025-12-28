//===--- GenericCompatibleTypesCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICCOMPATIBLETYPESCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICCOMPATIBLETYPESCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selections with compatible association types.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. Each association in a generic selection maps a
/// specific type to an expression. The C standard requires that association
/// types be compatible with each other according to C's type compatibility
/// rules.
///
/// This check enforces that no two association types in a generic selection
/// should be compatible. Compatible types would create ambiguity about which
/// association should be selected, leading to undefined behavior or compiler
/// errors.
///
/// Two types are compatible in C if:
/// - They are the same type
/// - They are compatible array types
/// - They are compatible pointer types
/// - They are compatible function types
/// - They are typedef aliases of compatible types
///
/// Related MISRA C:2025 Rule: 23.3 - Generic association types shall not be
/// compatible (Required).
///
/// Example:
/// \code
///   typedef int MyInt;
///
///   // Non-compliant: int and MyInt are compatible
///   _Generic(x, int: 1, MyInt: 2)
///
///   // Non-compliant: int* and int* are the same type
///   _Generic(x, int*: 1, int*: 2)
///
///   // Compliant: int and float are not compatible
///   _Generic(x, int: 1, float: 2)
/// \endcode
class GenericCompatibleTypesCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericCompatibleTypesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericCompatibleTypesCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICCOMPATIBLETYPESCHECK_H

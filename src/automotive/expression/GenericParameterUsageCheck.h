//===--- GenericParameterUsageCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICPARAMETERUSAGECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICPARAMETERUSAGECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selection parameters used more than once.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. The controlling expression determines which
/// association is selected based on its type. However, the controlling
/// expression is never evaluated - only its type is used for selection.
///
/// When a generic selection is used in a macro, care must be taken to ensure
/// that any parameters are not used multiple times within the controlling
/// expression or within the same association's result expression. Multiple
/// uses of the same parameter can lead to:
/// - Multiple evaluations if the parameter has side effects
/// - Unexpected behavior
/// - Performance issues
///
/// This check warns when a parameter appears multiple times in:
/// - The controlling expression
/// - Individual association expressions
///
/// Related MISRA C:2025 Rule: 23.8 - Generic selection parameters shall be
/// used at most once (Required).
///
/// Example:
/// \code
///   // Non-compliant: controlling expression uses x twice
///   #define BAD1(x) _Generic((x) + (x), int: 1, float: 2)
///
///   // Non-compliant: association expression uses x twice
///   #define BAD2(x) _Generic((x), int: (x) + (x), float: (x))
///
///   // Compliant: parameter used only once
///   #define GOOD(x) _Generic((x), int: process(x), float: handle(x))
///
///   // Non-compliant: x appears multiple times in controlling expression
///   #define MULTI(x) _Generic(sizeof(x) + sizeof(x), int: 1)
/// \endcode
class GenericParameterUsageCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericParameterUsageCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericParameterUsageCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Counts occurrences of each declaration in an expression.
  void countDeclRefs(const Expr *E, std::map<const ValueDecl *, unsigned> &Counts);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICPARAMETERUSAGECHECK_H

//===--- GenericParameterTypeCheckCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICPARAMETERTYPECHECKCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICPARAMETERTYPECHECKCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects generic selections where parameters are not type-checked in all
/// associations.
///
/// C11 introduced generic selections (_Generic) as a compile-time mechanism for
/// type-generic programming. When a generic selection is used in a function-like
/// macro, it's important that any parameters passed to the result expressions
/// undergo proper type checking for all possible association branches.
///
/// This check warns when a generic parameter appears in some association
/// expressions but not others, or when the parameter is used in ways that would
/// bypass type checking. This ensures consistent type safety across all branches
/// of the generic selection.
///
/// Specifically, this check looks for:
/// - Parameters used in some associations but missing in others
/// - Parameters used without proper type context
/// - Inconsistent parameter usage across associations
///
/// Related MISRA C:2025 Rule: 23.5 - A generic parameter should be type-checked
/// in all generic associations (Advisory).
///
/// Example:
/// \code
///   // Non-compliant: parameter x not used in all associations
///   #define PROCESS(x) _Generic((x), int: process_int(x), float: 0)
///
///   // Compliant: parameter x used consistently in all associations
///   #define PROCESS(x) _Generic((x), int: process_int(x), float: process_float(x))
///
///   // Non-compliant: different parameters used
///   #define MIXED(x, y) _Generic((x), int: use_x(x), float: use_y(y))
///
///   // Compliant: same parameter used in all associations
///   #define CONSISTENT(x) _Generic((x), int: use_int(x), float: use_float(x))
/// \endcode
class GenericParameterTypeCheckCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  GenericParameterTypeCheckCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~GenericParameterTypeCheckCheck() override = default;

  /// Registers AST matchers for generic selection expressions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched generic selection expressions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Collects all DeclRefExpr nodes from an expression tree.
  void collectDeclRefs(const Expr *E,
                       llvm::SmallVectorImpl<const DeclRefExpr *> &DeclRefs);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_GENERICPARAMETERTYPECHECKCHECK_H

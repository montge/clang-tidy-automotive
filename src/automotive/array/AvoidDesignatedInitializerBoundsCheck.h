//===--- AvoidDesignatedInitializerBoundsCheck.h - clang-tidy ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDDESIGNATEDINITIALIZERBOUNDSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDDESIGNATEDINITIALIZERBOUNDSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects designated initializers with out-of-bounds array indices.
///
/// MISRA C:2025 Rule 9.5 requires that designated initializers shall not
/// specify array indices that exceed the declared array bounds or reference
/// non-existent struct members.
///
/// Example of non-compliant code:
/// \code
///   int arr[3] = { [5] = 10 };  // Index 5 exceeds array bounds
/// \endcode
///
/// Example of compliant code:
/// \code
///   int arr[3] = { [2] = 10 };  // Valid index within bounds
/// \endcode
class AvoidDesignatedInitializerBoundsCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  AvoidDesignatedInitializerBoundsCheck(StringRef Name,
                                        ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidDesignatedInitializerBoundsCheck() override = default;

  /// Registers AST matchers for designated initializers.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched initializers and emits diagnostics.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check a single array designator for bounds issues.
  void
  checkArrayDesignator(const DesignatedInitExpr::Designator &D,
                       const DesignatedInitExpr *DIE,
                       const ast_matchers::MatchFinder::MatchResult &Result);

  /// Check a GNU array range designator for bounds issues.
  void checkArrayRangeDesignator(
      const DesignatedInitExpr::Designator &D, const DesignatedInitExpr *DIE,
      const ast_matchers::MatchFinder::MatchResult &Result);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDDESIGNATEDINITIALIZERBOUNDSCHECK_H

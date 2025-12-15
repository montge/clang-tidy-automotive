//===--- MultipleReturnStmtCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MULTIPLERETURNSTMTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MULTIPLERETURNSTMTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects functions with multiple return statements.
///
/// Functions should have a single return point to simplify control flow
/// and make the code easier to understand, debug, and verify. Multiple
/// return statements can make it harder to track all exit paths and ensure
/// proper resource cleanup before function exit.
///
/// Related MISRA C:2025 Rule: 15.5 - A function should have a single point
/// of exit at the end.
///
/// Example:
/// \code
///   int func(int x) {
///     if (x < 0) return -1;  // Warning: multiple returns
///     return x * 2;
///   }
/// \endcode
class MultipleReturnStmtCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  MultipleReturnStmtCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~MultipleReturnStmtCheck() override = default;

  /// Registers AST matchers for return statements.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched return statements and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  const FunctionDecl *CurrentFunc = nullptr;
  const ReturnStmt *PreviousReturn = nullptr;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MULTIPLERETURNSTMTCHECK_H

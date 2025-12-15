//===--- MissingDefaultInSwitchStmtCheck.h - clang-tidy ----*- C++ -*------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGDEFAULTINSWITCHSTMTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGDEFAULTINSWITCHSTMTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects switch statements without a default case.
///
/// Every switch statement should have a default case to handle unexpected
/// values and ensure defensive programming. This helps catch logic errors
/// and makes the code more robust against future enum additions.
///
/// Related MISRA C:2025 Rule: 16.4 - Every switch statement shall have a
/// default label.
///
/// Example:
/// \code
///   void func(int x) {
///     switch (x) {  // Warning: switch missing default case
///       case 1: break;
///       case 2: break;
///     }
///   }
/// \endcode
class MissingDefaultInSwitchStmtCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  MissingDefaultInSwitchStmtCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~MissingDefaultInSwitchStmtCheck() override = default;

  /// Registers AST matchers for switch statements.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched switch statements and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_MISSINGDEFAULTINSWITCHSTMTCHECK_H

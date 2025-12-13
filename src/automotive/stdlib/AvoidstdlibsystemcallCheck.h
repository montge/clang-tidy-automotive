//===--- AvoidstdlibsystemcallCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBSYSTEMCALLCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBSYSTEMCALLCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of the system() function from stdlib.h.
///
/// The system() function executes external commands, which poses security
/// risks and makes program behavior dependent on the environment. It can
/// lead to command injection vulnerabilities and makes the program's behavior
/// unpredictable and difficult to verify in safety-critical systems.
///
/// Related MISRA C:2025 Rule: 21.8 - The library functions system and abort
/// shall not be used.
///
/// Example:
/// \code
///   system("ls");  // Warning: system() function call
/// \endcode
class AvoidstdlibsystemcallCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidstdlibsystemcallCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Registers AST matchers for system() calls.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched system() calls and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDLIBSYSTEMCALLCHECK_H

//===--- AvoidTempArrayDecayCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTEMPARRAYDECAYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTEMPARRAYDECAYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects array-to-pointer conversion of objects with temporary lifetime.
///
/// An object with temporary lifetime (such as a compound literal with array
/// type) that undergoes array-to-pointer conversion creates a pointer to
/// memory that may be deallocated when the temporary's lifetime ends. This
/// can lead to dangling pointers and undefined behavior.
///
/// Related MISRA C:2025 Rule: 18.9 - An object with temporary lifetime shall
/// not undergo array-to-pointer conversion.
///
/// Example:
/// \code
///   void func(void) {
///     int *p = (int[]){1, 2, 3};  // Warning: compound literal decays to
///     pointer
///   }
/// \endcode
class AvoidTempArrayDecayCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidTempArrayDecayCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidTempArrayDecayCheck() override = default;

  /// Registers AST matchers for compound literals undergoing array-to-pointer
  /// conversion.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched compound literal conversions and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTEMPARRAYDECAYCHECK_H

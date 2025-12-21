//===--- AvoidTentativeDefinitionInHeaderCheck.h - clang-tidy --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTENTATIVEDEFINITIONINHEADERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTENTATIVEDEFINITIONINHEADERCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects tentative definitions in header files.
///
/// A tentative definition is a declaration of an object at file scope without
/// an initializer. In header files, this can lead to multiple definition
/// errors when the header is included in multiple translation units.
///
/// Related MISRA C:2025 Rule: 8.18
///
/// Example:
/// \code
///   // In header.h:
///   int global_var;       // Warning: tentative definition in header
///   int initialized = 0;  // OK: explicit definition
///   extern int declared;  // OK: declaration only
/// \endcode
class AvoidTentativeDefinitionInHeaderCheck : public ClangTidyCheck {
public:
  AvoidTentativeDefinitionInHeaderCheck(StringRef Name,
                                        ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTENTATIVEDEFINITIONINHEADERCHECK_H

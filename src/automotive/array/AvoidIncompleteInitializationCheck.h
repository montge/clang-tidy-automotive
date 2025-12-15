//===--- AvoidIncompleteInitializationCheck.h - clang-tidy ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPLETEINITIALIZATIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPLETEINITIALIZATIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects incomplete aggregate initialization.
///
/// MISRA C:2025 Rule 9.4 requires that if any element of an aggregate
/// (array or struct) is initialized, then all elements shall be explicitly
/// initialized. This prevents accidental reliance on implicit zero
/// initialization.
///
/// Example of non-compliant code:
/// \code
///   struct Point { int x; int y; int z; };
///   struct Point p = { .x = 1, .y = 2 }; // z not initialized
///   int arr[5] = { 1, 2 }; // Elements 2-4 not initialized
/// \endcode
///
/// Example of compliant code:
/// \code
///   struct Point p = { .x = 1, .y = 2, .z = 0 };
///   int arr[5] = { 1, 2, 0, 0, 0 };
/// \endcode
class AvoidIncompleteInitializationCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  AvoidIncompleteInitializationCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidIncompleteInitializationCheck() override = default;

  /// Registers AST matchers for aggregate initializations.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched initializers and emits diagnostics.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Gets the number of fields in a record type.
  unsigned getRecordFieldCount(const RecordType *RT) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPLETEINITIALIZATIONCHECK_H

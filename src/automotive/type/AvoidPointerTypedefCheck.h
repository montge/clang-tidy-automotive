//===--- AvoidPointerTypedefCheck.h - clang-tidy ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTYPEDEFCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTYPEDEFCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects pointer types hidden in typedefs.
///
/// MISRA C:2025 Dir 4.8 (Advisory) recommends that pointers should not be
/// hidden in typedefs. This helps ensure that the pointer nature of a type
/// is visible at the point of use, improving code clarity and reducing the
/// risk of pointer-related errors.
///
/// Example of non-compliant code:
/// \code
///   typedef int* IntPtr;   // Pointer hidden in typedef
///   IntPtr p = NULL;       // Not obvious that p is a pointer
/// \endcode
///
/// Example of compliant code:
/// \code
///   typedef struct Data Data_t;
///   Data_t *p = NULL;      // Pointer nature is explicit
/// \endcode
///
/// Note: This check may have false positives for opaque handles that
/// intentionally hide implementation details.
class AvoidPointerTypedefCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  AvoidPointerTypedefCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidPointerTypedefCheck() override = default;

  /// Registers AST matchers for typedef declarations.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched typedefs and emits diagnostics.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check a typedef declaration for hidden pointer.
  void checkTypedefDecl(const TypedefDecl *TD, const SourceManager &SM);

  /// Check a type alias declaration for hidden pointer.
  void checkTypeAliasDecl(const TypeAliasDecl *TAD, const SourceManager &SM);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDPOINTERTYPEDEFCHECK_H

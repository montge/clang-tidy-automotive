//===--- AvoidConstQualifierRemovalCheck.h - clang-tidy --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCONSTQUALIFIERREMOVALCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCONSTQUALIFIERREMOVALCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-pointer
/// @brief Detects casts that remove const or volatile qualifiers from pointers.
///
/// A cast shall not remove const or volatile qualification from the type
/// pointed to by a pointer. Casting away const allows modification of objects
/// that were intended to be read-only, potentially causing undefined behavior.
/// Casting away volatile may cause the compiler to optimize away necessary
/// accesses to memory-mapped I/O or shared memory locations.
///
/// @par MISRA C:2025 Rule 11.8
/// A cast shall not remove const or volatile qualification from the type
/// pointed to by a pointer.
/// @par Category: Required
///
/// Example violations:
/// @code
///   const int *pc = &x;
///   int *p = (int *)pc;          // Warning: removes const
///
///   volatile int *pv = &v;
///   int *q = (int *)pv;          // Warning: removes volatile
///
///   const volatile int *pcv;
///   int *r = (int *)pcv;         // Warning: removes const and volatile
///   const int *s = (const int *)pcv;  // Warning: removes volatile
/// @endcode
///
/// Example compliant code:
/// @code
///   int *p = &x;
///   const int *pc = (const int *)p;  // OK: adding const
///
///   const int *pc2 = &y;
///   const int *pc3 = (const int *)pc2;  // OK: same qualifiers
/// @endcode
class AvoidConstQualifierRemovalCheck : public ClangTidyCheck {
public:
  AvoidConstQualifierRemovalCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidConstQualifierRemovalCheck() override = default;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCONSTQUALIFIERREMOVALCHECK_H

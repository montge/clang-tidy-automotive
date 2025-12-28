//===--- AvoidOutOfBoundsAccessCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOUTOFBOUNDSACCESSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOUTOFBOUNDSACCESSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-arrays
/// @brief Detects out-of-bounds array accesses.
///
/// Accessing an array element outside its bounds is undefined behavior and
/// can lead to crashes, data corruption, and security vulnerabilities.
/// This check detects statically-determinable out-of-bounds accesses.
///
/// @par MISRA C++:2023 Rule 13.1
/// An object shall not be accessed out of bounds.
/// @par Category: Required
///
/// Example:
/// @code
///   // Non-compliant:
///   int arr[5];
///   int x = arr[5];   // Warning - index 5 is out of bounds [0, 4]
///   int y = arr[10];  // Warning - index 10 is out of bounds
///
///   // Compliant:
///   int z = arr[4];   // OK - valid index
///   int w = arr[0];   // OK
/// @endcode
class AvoidOutOfBoundsAccessCheck : public ClangTidyCheck {
public:
  AvoidOutOfBoundsAccessCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidOutOfBoundsAccessCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkArraySubscript(const ArraySubscriptExpr *ASE, ASTContext *Context);
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDOUTOFBOUNDSACCESSCHECK_H

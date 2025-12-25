//===--- AvoidVoidPointerCastCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVOIDPOINTERCASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVOIDPOINTERCASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-expressions
/// @brief Detects conversions from void* to typed pointers.
///
/// Conversions from void* to typed pointers should be explicit and verified
/// to prevent type safety issues. Implicit conversions and unverified explicit
/// conversions can lead to undefined behavior if the pointer does not actually
/// point to the target type.
///
/// @par MISRA C++:2023 Rule 10.2
/// Conversions from void* to typed pointer should be explicit and verified.
/// @par Category: Required
///
/// Example:
/// @code
///   void* vp = malloc(sizeof(int));
///   int* ip = static_cast<int*>(vp);  // Warning: unverified cast
///
///   // Better: verify with runtime checks or use typed allocation
///   int* ip2 = new int;  // OK: typed allocation
/// @endcode
class AvoidVoidPointerCastCheck : public ClangTidyCheck {
public:
  AvoidVoidPointerCastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidVoidPointerCastCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check if the cast is part of a sizeof comparison or type verification
  bool hasTypeVerification(const Expr *CastExpr, ASTContext *Context) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVOIDPOINTERCASTCHECK_H

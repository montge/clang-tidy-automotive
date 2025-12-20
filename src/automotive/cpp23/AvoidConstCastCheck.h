//===--- AvoidConstCastCheck.h - clang-tidy ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTCASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTCASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-expressions
/// @brief Detects usage of const_cast.
///
/// const_cast can be used to cast away const or volatile qualifiers, which
/// can lead to undefined behavior if the underlying object is actually const.
/// It should be avoided in safety-critical code.
///
/// @par MISRA C++:2023 Rule 8.2.3
/// const_cast shall not be used.
/// @par Category: Required
///
/// Example:
/// @code
///   const int x = 42;
///   int* p = const_cast<int*>(&x);  // Warning
///   *p = 10;  // Undefined behavior!
/// @endcode
class AvoidConstCastCheck : public ClangTidyCheck {
public:
  AvoidConstCastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidConstCastCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTCASTCHECK_H

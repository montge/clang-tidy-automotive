//===--- AvoidReinterpretCastCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDREINTERPRETCASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDREINTERPRETCASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-expressions
/// @brief Detects usage of reinterpret_cast.
///
/// reinterpret_cast performs low-level type reinterpretation with no safety
/// checks. It can easily lead to undefined behavior and should be avoided
/// in safety-critical code.
///
/// @par MISRA C++:2023 Rule 8.2.4
/// reinterpret_cast shall not be used.
/// @par Category: Required
///
/// Example:
/// @code
///   int x = 42;
///   float* fp = reinterpret_cast<float*>(&x);  // Warning
/// @endcode
class AvoidReinterpretCastCheck : public ClangTidyCheck {
public:
  AvoidReinterpretCastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidReinterpretCastCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDREINTERPRETCASTCHECK_H

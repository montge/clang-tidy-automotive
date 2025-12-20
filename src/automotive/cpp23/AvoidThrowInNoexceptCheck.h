//===--- AvoidThrowInNoexceptCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTHROWINNOEXCEPTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTHROWINNOEXCEPTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-exceptions
/// @brief Detects throw expressions inside noexcept functions.
///
/// A throw expression in a noexcept function will result in std::terminate
/// being called if the exception propagates. This is often unintended and
/// indicates a design issue.
///
/// @par MISRA C++:2023 Rule 18.4.3
/// A noexcept function shall not throw.
/// @par Category: Required
///
/// Example:
/// @code
///   void foo() noexcept {
///     throw std::runtime_error("error");  // Warning: throw in noexcept
///   }
/// @endcode
class AvoidThrowInNoexceptCheck : public ClangTidyCheck {
public:
  AvoidThrowInNoexceptCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidThrowInNoexceptCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11; // noexcept requires C++11
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTHROWINNOEXCEPTCHECK_H

//===--- AvoidDynamicCastCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDYNAMICCASTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDYNAMICCASTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-expressions
/// @brief Detects usage of dynamic_cast.
///
/// dynamic_cast relies on RTTI and can fail at runtime, throwing std::bad_cast
/// for references or returning nullptr for pointers. In safety-critical
/// systems, the runtime overhead and potential for failure make it undesirable.
///
/// @par MISRA C++:2023 Rule 8.2.5
/// dynamic_cast should not be used.
/// @par Category: Advisory
///
/// Example:
/// @code
///   Base* base = new Derived();
///   Derived* d = dynamic_cast<Derived*>(base);  // Warning: dynamic_cast used
/// @endcode
class AvoidDynamicCastCheck : public ClangTidyCheck {
public:
  AvoidDynamicCastCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidDynamicCastCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDYNAMICCASTCHECK_H

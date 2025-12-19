//===--- AvoidThrowingDestructorCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTHROWINGDESTRUCTORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTHROWINGDESTRUCTORCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-exceptions
/// @brief Detects destructors that may throw exceptions.
///
/// Destructors should not throw exceptions because they are often called during
/// stack unwinding when another exception is active. Throwing in this context
/// results in std::terminate being called.
///
/// @par MISRA C++:2023 Rule 18.4.1
/// Destructors shall not throw exceptions.
/// @par Category: Required
///
/// Example:
/// @code
///   class Bad {
///     ~Bad() { throw std::runtime_error("error"); }  // Warning
///   };
/// @endcode
class AvoidThrowingDestructorCheck : public ClangTidyCheck {
public:
  AvoidThrowingDestructorCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidThrowingDestructorCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTHROWINGDESTRUCTORCHECK_H

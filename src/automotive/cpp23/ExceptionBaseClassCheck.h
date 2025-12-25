//===--- ExceptionBaseClassCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_EXCEPTIONBASECLASSCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_EXCEPTIONBASECLASSCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-exceptions
/// @brief Detects exceptions that don't derive from std::exception.
///
/// Throwing exceptions that do not derive from std::exception makes it harder
/// to catch exceptions consistently and can lead to maintenance issues.
/// Only std::exception and its derived types should be thrown.
///
/// @par MISRA C++:2023 Rule 18.3.1
/// Exceptions shall only be of types that derive from std::exception.
/// @par Category: Required
///
/// Example:
/// @code
///   throw 42;                    // Warning: throwing primitive type
///   throw "error";               // Warning: throwing C-string
///   class MyError {};
///   throw MyError();             // Warning: not derived from std::exception
///
///   class MyException : public std::exception {};
///   throw MyException();         // OK
///   throw std::runtime_error(""); // OK
/// @endcode
class ExceptionBaseClassCheck : public ClangTidyCheck {
public:
  ExceptionBaseClassCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~ExceptionBaseClassCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus; // Exceptions require C++
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_EXCEPTIONBASECLASSCHECK_H

//===--- AvoidCArrayCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCARRAYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCARRAYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects C-style array declarations in C++ code.
///
/// std::array or std::vector should be used instead of a C-style array.
/// C-style arrays do not provide bounds checking and can lead to buffer
/// overflows and other safety issues. Modern C++ containers provide safer
/// alternatives with better expressiveness and safety guarantees.
///
/// Related MISRA C++:2023 Rule 11.3.1 (Advisory)
///
/// Example:
/// \code
///   int arr[10];              // Warning: use std::array instead
///   int matrix[3][3];         // Warning: use std::array instead
///   void func(int arr[]) {}   // Warning: use std::vector or std::span instead
/// \endcode
///
/// Exceptions:
/// - Arrays in system headers are ignored
/// - Arrays in extern "C" contexts are allowed (for C interoperability)
/// - Main function's argv parameter is allowed
class AvoidCArrayCheck : public ClangTidyCheck {
public:
  AvoidCArrayCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDCARRAYCHECK_H

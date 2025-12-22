//===--- AvoidReallocCheck.h - clang-tidy ------------------------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDREALLOCCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDREALLOCCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-stdlib
/// @brief Detects use of realloc function.
///
/// The realloc function from <cstdlib> shall not be used as it can lead
/// to memory leaks, use-after-free, and undefined behavior.
///
/// @par MISRA C++:2023 Rule 19.3 (Required)
/// The <cstdlib> function realloc shall not be used.
///
/// Example:
/// @code
///   // Non-compliant:
///   void* ptr = std::malloc(10);
///   ptr = std::realloc(ptr, 20);  // realloc usage
///
///   // Compliant:
///   void* ptr = std::malloc(20);  // Allocate correct size initially
///   // Or use C++ containers that manage memory automatically
/// @endcode
class AvoidReallocCheck : public ClangTidyCheck {
public:
  AvoidReallocCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDREALLOCCHECK_H

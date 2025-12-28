//===--- AvoidStaticInitAllocationCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDSTATICINITALLOCATIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDSTATICINITALLOCATIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-memory
/// @brief Detects dynamic memory allocation during static initialization.
///
/// Dynamic memory allocation during static initialization can lead to issues
/// with initialization order and can cause failures that are difficult to debug.
/// Memory allocated during static initialization may not be properly cleaned up,
/// leading to resource leaks.
///
/// @par MISRA C++:2023 Rule 10.1
/// Memory shall not be allocated dynamically during static initialization.
/// @par Category: Required
///
/// Example:
/// @code
///   // Non-compliant:
///   int* global_ptr = new int(42);  // Warning
///   std::vector<int> global_vec = {1, 2, 3};  // May allocate
///
///   // Compliant:
///   int global_value = 42;
///   int* global_ptr = nullptr;
///   void init() { global_ptr = new int(42); }
/// @endcode
class AvoidStaticInitAllocationCheck : public ClangTidyCheck {
public:
  AvoidStaticInitAllocationCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidStaticInitAllocationCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isInStaticInitializer(const Expr *E, ASTContext *Context) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDSTATICINITALLOCATIONCHECK_H

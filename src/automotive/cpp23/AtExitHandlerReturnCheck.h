//===--- AtExitHandlerReturnCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_ATEXITHANDLERRETURNCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_ATEXITHANDLERRETURNCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-functions
/// @brief Ensures at-exit handlers terminate by returning normally.
///
/// Functions registered with std::atexit or std::at_quick_exit should not
/// throw exceptions, call std::exit, std::abort, or std::longjmp. These
/// functions should terminate normally by returning.
///
/// @par MISRA C++:2023 Rule 10.2
/// At-exit handlers shall terminate by returning.
/// @par Category: Required
///
/// Example:
/// @code
///   // Non-compliant:
///   void exit_handler1() {
///     throw std::runtime_error("error");  // Warning
///   }
///
///   void exit_handler2() {
///     std::exit(1);  // Warning
///   }
///
///   // Compliant:
///   void exit_handler3() {
///     cleanup();
///     return;  // OK - normal return
///   }
/// @endcode
class AtExitHandlerReturnCheck : public ClangTidyCheck {
public:
  AtExitHandlerReturnCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AtExitHandlerReturnCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkAtExitFunction(const FunctionDecl *FD, ASTContext *Context);
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_ATEXITHANDLERRETURNCHECK_H

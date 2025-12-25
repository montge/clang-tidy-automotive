//===--- UninitializedVariableCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNINITIALIZEDVARIABLECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNINITIALIZEDVARIABLECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-declaration
/// @brief Detects variables that are not initialized before first use.
///
/// All variables shall be initialized before their first use. This includes:
/// - Local variables (automatic storage duration)
/// - Class member variables (in constructor initializer lists)
/// - Static variables
///
/// @par MISRA C++:2023 Rule 11.6.1 (Required)
/// All variables shall be initialized before first use.
///
/// Example:
/// @code
///   // Compliant:
///   int x = 0;
///   int y{5};
///   std::string s;  // OK - default constructor initializes
///
///   // Non-compliant:
///   int x;          // Uninitialized local variable
///   static int z;   // Uninitialized static variable
///
///   class MyClass {
///     int member;   // Uninitialized if not in constructor initializer list
///   public:
///     MyClass() {}  // Non-compliant - member not initialized
///   };
/// @endcode
class UninitializedVariableCheck : public ClangTidyCheck {
public:
  UninitializedVariableCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkLocalVariable(const VarDecl *Var, const SourceManager *SM);
  void checkConstructor(const CXXConstructorDecl *Ctor,
                        const SourceManager *SM);
  bool hasInitializingConstructor(QualType Type) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_UNINITIALIZEDVARIABLECHECK_H

//===--- AvoidUnusedVariadicTemplateCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNUSEDVARIADICTEMPLATECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNUSEDVARIADICTEMPLATECHECK_H

#include "../../ClangTidyCheck.h"
#include <map>
#include <set>

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-templates
/// @brief Detects variadic templates that are never instantiated with variadic
/// arguments.
///
/// A variadic template that is never used with variadic arguments is
/// unnecessarily complex and should use regular templates instead. This check
/// warns when a variadic template has at least one instantiation but all
/// instantiations have empty parameter packs.
///
/// @par MISRA C++:2023 Rule 16.6.1
/// A variadic template shall have at least one instantiation with at least one
/// variadic argument.
/// @par Category: Advisory
///
/// Example:
/// @code
///   // Warning: variadic template never used with args
///   template<typename... Args>
///   void unused_variadic(Args...) {}
///
///   void foo() {
///     unused_variadic();  // Only called with empty pack
///   }
///
///   // Warning: class template only instantiated with single arg
///   template<typename T, typename... Rest>
///   class Container {};
///
///   Container<int> c;  // If only used with single arg, warn
///
///   // OK: variadic template used with variadic args
///   template<typename... Args>
///   void print(Args... args) {}
///
///   void bar() {
///     print(1, 2, 3);  // Used with multiple arguments
///   }
/// @endcode
class AvoidUnusedVariadicTemplateCheck : public ClangTidyCheck {
public:
  AvoidUnusedVariadicTemplateCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidUnusedVariadicTemplateCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11; // Variadic templates introduced in C++11
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

private:
  // Track variadic templates and their instantiations
  struct TemplateInfo {
    SourceLocation Loc;
    std::string Name;
    bool HasVariadicInstantiation = false;
    bool HasAnyInstantiation = false;
  };

  std::map<const Decl *, TemplateInfo> VariadicTemplates;

  bool hasVariadicParameter(const TemplateParameterList *TPL) const;
  unsigned countVariadicArgs(const TemplateArgumentList *Args) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNUSEDVARIADICTEMPLATECHECK_H

//===--- UnconstrainedTemplateCheck.h - clang-tidy --------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_UNCONSTRAINEDTEMPLATECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_UNCONSTRAINEDTEMPLATECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-templates
/// @brief Detects template functions and classes without type constraints.
///
/// Templates without proper constraints can be instantiated with inappropriate
/// types, leading to confusing error messages or undefined behavior. This check
/// encourages the use of concepts (C++20) or static_assert for type
/// constraints.
///
/// @par MISRA C++:2023 Rule 17.0.1
/// Template parameters shall be constrained.
/// @par Category: Advisory
///
/// Example:
/// @code
///   // Warning: unconstrained template
///   template <typename T>
///   T add(T a, T b) { return a + b; }
///
///   // OK: constrained with concept (C++20)
///   template <typename T>
///   requires std::integral<T>
///   T add(T a, T b) { return a + b; }
///
///   // OK: constrained with static_assert
///   template <typename T>
///   T add(T a, T b) {
///     static_assert(std::is_arithmetic_v<T>, "T must be arithmetic");
///     return a + b;
///   }
/// @endcode
class UnconstrainedTemplateCheck : public ClangTidyCheck {
public:
  UnconstrainedTemplateCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~UnconstrainedTemplateCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool hasConstraints(const FunctionTemplateDecl *FTD) const;
  bool hasConstraints(const ClassTemplateDecl *CTD) const;
  bool hasStaticAssert(const FunctionDecl *FD) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_UNCONSTRAINEDTEMPLATECHECK_H

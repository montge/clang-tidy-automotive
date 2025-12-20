//===--- ProperConceptDefinitionCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_PROPERCONCEPTDEFINITIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_PROPERCONCEPTDEFINITIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-cpp23-templates
/// @brief Ensures C++20 concepts are properly defined and meaningful.
///
/// Concept definitions should be atomic (testing one thing), composable,
/// and clearly documented. This check detects concepts that may be too
/// broad or trivially satisfied.
///
/// @par MISRA C++:2023 Rule 17.1.1
/// Concepts shall be properly constrained.
/// @par Category: Advisory
///
/// Example:
/// @code
///   // Warning: trivially satisfied concept
///   template <typename T>
///   concept AlwaysTrue = true;
///
///   // OK: meaningful constraint
///   template <typename T>
///   concept Addable = requires(T a, T b) { a + b; };
/// @endcode
class ProperConceptDefinitionCheck : public ClangTidyCheck {
public:
  ProperConceptDefinitionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~ProperConceptDefinitionCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus20;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isTrivialConstraint(const Expr *E) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_PROPERCONCEPTDEFINITIONCHECK_H

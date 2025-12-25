//===--- AvoidTemporaryBindingToNTTPCheck.h - clang-tidy -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTEMPORARYBINDINGTONTTPCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTEMPORARYBINDINGTONTTPCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects non-type template parameters of reference type that bind to
/// temporaries.
///
/// MISRA C++:2023 Rule 16.6 (Advisory): A non-type template parameter of
/// reference type should not bind to a temporary.
///
/// When a temporary is used to instantiate a template with a reference
/// parameter, the reference becomes dangling after the temporary is destroyed,
/// leading to undefined behavior.
///
/// This check detects:
/// - Template instantiations where a temporary binds to a reference parameter
/// - Implicit temporary creation in template arguments (e.g., literals)
/// - Rvalue temporaries binding to const reference parameters
///
/// Example violations:
/// @code
///   template<const int& N>
///   struct S {};
///
///   S<42> s;  // Warning: literal binds to reference parameter
/// @endcode
///
/// Compliant examples:
/// @code
///   template<const int& N>
///   struct S {};
///
///   int x = 42;
///   S<x> s1;  // OK: binds to named variable
///
///   static int y = 42;
///   S<y> s2;  // OK: binds to static variable
/// @endcode
class AvoidTemporaryBindingToNTTPCheck : public ClangTidyCheck {
public:
  AvoidTemporaryBindingToNTTPCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check if an expression is a temporary or prvalue
  bool isTemporaryOrPRValue(const Expr *E) const;

  /// Check if a template argument binds a temporary to a reference parameter
  bool bindsTemporaryToReference(const TemplateArgument &Arg,
                                 const NonTypeTemplateParmDecl *Param,
                                 const ASTContext &Context) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTEMPORARYBINDINGTONTTPCHECK_H

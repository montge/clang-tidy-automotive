//===--- AvoidTemplateRefTemporaryCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTEMPLATEREFTEMPORARYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTEMPLATEREFTEMPORARYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-templates
/// @brief Detects non-type template parameters of reference type that bind to
/// temporaries.
///
/// A non-type template parameter of reference type shall not bind to a
/// temporary object. When a temporary is used to instantiate a template with
/// a reference parameter, the reference becomes dangling after the temporary
/// is destroyed, leading to undefined behavior.
///
/// @par MISRA C++:2023 Rule 16.6
/// A non-type template parameter of reference type should not bind to a
/// temporary.
/// @par Category: Advisory
///
/// This check detects:
/// - Template instantiations where a temporary binds to a reference parameter
/// - Implicit temporary creation in template arguments
/// - Rvalue temporaries binding to non-const reference parameters
///
/// Example:
/// @code
///   template<int& N>
///   void process() {
///     // Use N...
///   }
///
///   int global = 42;
///   process<global>();  // OK: binds to lvalue
///
///   int getValue() { return 42; }
///   process<getValue()>();  // Warning: temporary binds to reference parameter
///
///   // Also problematic:
///   template<const int& N>
///   void processConst() {
///     // Use N...
///   }
///
///   processConst<42>();  // Warning: temporary literal binds to reference
/// @endcode
class AvoidTemplateRefTemporaryCheck : public ClangTidyCheck {
public:
  AvoidTemplateRefTemporaryCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidTemplateRefTemporaryCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check if an expression is a temporary
  bool isTemporary(const Expr *E) const;

  /// Check if a template argument refers to a temporary
  bool templateArgumentBindsToTemporary(const TemplateArgument &Arg,
                                        const ASTContext &Context) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDTEMPLATEREFTEMPORARYCHECK_H

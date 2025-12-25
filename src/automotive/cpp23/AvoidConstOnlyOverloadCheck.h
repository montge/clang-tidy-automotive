//===--- AvoidConstOnlyOverloadCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTONLYOVERLOADCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTONLYOVERLOADCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects overloaded functions that differ only in top-level const
/// qualification of parameters.
///
/// MISRA C++:2023 Rule 11.3.2: Overloaded functions should differ in parameter
/// types, not just in parameter const qualification.
///
/// Functions that only differ in top-level const/volatile qualification can't
/// be overloaded and cause confusion. Top-level const on parameters is ignored
/// for function signature matching.
///
/// Example violations:
/// @code
///   void func(int x);
///   void func(const int x);  // Warning: differs only in const
///
///   class C {
///     void method(double d);
///     void method(const double d);  // Warning
///   };
/// @endcode
///
/// Compliant:
/// @code
///   void func(int x);
///   void func(int& x);  // OK - different type
///
///   void func(const int* p);  // OK - const is not top-level
///   void func(int* p);
/// @endcode
class AvoidConstOnlyOverloadCheck : public ClangTidyCheck {
public:
  AvoidConstOnlyOverloadCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

private:
  /// Checks if two types differ only in top-level const/volatile qualification.
  /// Returns true if types are the same except for top-level cv-qualifiers.
  bool differOnlyInTopLevelQualifiers(QualType T1, QualType T2) const;

  /// Track functions already reported to avoid duplicate warnings
  llvm::DenseSet<const FunctionDecl *> ReportedFunctions;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDCONSTONLYOVERLOADCHECK_H

//===--- AvoidDanglingReferenceCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDANGLINGREFERENCECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDANGLINGREFERENCECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-functions
/// @brief Detects functions that return references or pointers to automatic
/// variables.
///
/// A function shall not return a reference or a pointer to an automatic
/// variable (local variable) defined in the same function. This creates a
/// dangling reference or pointer that refers to a destroyed object, leading to
/// undefined behavior.
///
/// @par MISRA C++:2023 Rule 7.5.1
/// A function shall not return a reference or a pointer to an automatic
/// variable.
/// @par Category: Required
///
/// This check detects:
/// - Return statements returning reference to local variable
/// - Return statements returning pointer to local variable
/// - Return statements returning address of local variable (&local)
/// - Lambda captures returning references to captured locals
///
/// Example:
/// @code
///   int* bad_function() {
///     int local = 42;
///     return &local;  // Warning: returning address of local variable
///   }
///
///   int& bad_reference() {
///     int local = 42;
///     return local;  // Warning: returning reference to local variable
///   }
///
///   // Good: return by value or return pointer to heap/static storage
///   int good_function() {
///     int local = 42;
///     return local;  // OK: return by value
///   }
///
///   int* good_pointer() {
///     static int persistent = 42;
///     return &persistent;  // OK: static storage duration
///   }
/// @endcode
class AvoidDanglingReferenceCheck : public ClangTidyCheck {
public:
  AvoidDanglingReferenceCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~AvoidDanglingReferenceCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check if a variable is a local variable with automatic storage duration
  bool isLocalAutoVariable(const VarDecl *VD) const;

  /// Check if the return value refers to a local variable
  const VarDecl *getReferencedLocalVar(const Expr *E) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDDANGLINGREFERENCECHECK_H

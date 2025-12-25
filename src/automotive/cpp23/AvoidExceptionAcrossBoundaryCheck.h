//===--- AvoidExceptionAcrossBoundaryCheck.h - clang-tidy ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDEXCEPTIONACROSSBOUNDARYCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDEXCEPTIONACROSSBOUNDARYCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-exceptions
/// @brief Detects exceptions thrown across execution boundaries.
///
/// Exceptions shall not be thrown across execution boundaries such as:
/// - Functions with C linkage (extern "C")
/// - Functions without proper exception specifications
/// - DLL/shared library boundaries (functions with external linkage)
/// - Callback functions passed to C APIs
///
/// Throwing exceptions across such boundaries leads to undefined behavior
/// because the calling code may not be prepared to handle C++ exceptions,
/// or the exception handling mechanisms may not be compatible.
///
/// @par MISRA C++:2023 Rule 16.0.1
/// Exceptions shall not be thrown across execution boundaries.
/// @par Category: Required
///
/// Example violations:
/// @code
///   extern "C" void callback() {
///     throw std::runtime_error("error");  // Warning: throw in extern "C"
///   }
///
///   void foo() noexcept(false) {
///     throw 42;  // Warning: function without noexcept may cross boundaries
///   }
///
///   __attribute__((visibility("default"))) void exported() {
///     throw std::exception();  // Warning: exported function may cross DLL
///                              // boundary
///   }
/// @endcode
///
/// Compliant:
/// @code
///   extern "C" void callback() noexcept {
///     // No exceptions thrown
///   }
///
///   void foo() noexcept {
///     // No exceptions thrown
///   }
/// @endcode
class AvoidExceptionAcrossBoundaryCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidExceptionAcrossBoundaryCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidExceptionAcrossBoundaryCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for throw expressions in boundary-crossing
  /// functions.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched throw expressions and checks for boundary violations.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Checks if a function has external linkage that could cross DLL boundaries.
  /// \param Func The function declaration to check.
  /// \return true if the function has external linkage.
  bool hasExternalLinkage(const FunctionDecl *Func) const;

  /// Checks if a function might be a callback passed to C code.
  /// \param Func The function declaration to check.
  /// \return true if the function might be used as a C callback.
  bool mightBeCCallback(const FunctionDecl *Func) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDEXCEPTIONACROSSBOUNDARYCHECK_H

//===--- AvoidUnrelatedPointerConversionCheck.h - clang-tidy ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNRELATEDPOINTERCONVERSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNRELATEDPOINTERCONVERSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-expressions
/// @brief Detects pointer conversions between unrelated class types.
///
/// This check detects static_cast or reinterpret_cast operations that convert
/// pointers between class types that are not in the same inheritance hierarchy.
/// Such conversions are dangerous and typically indicate a design flaw.
///
/// @par MISRA C++:2023 Rule 10.1
/// Pointer conversions between classes that are not in the same hierarchy
/// should be avoided.
/// @par Category: Required
///
/// Example:
/// @code
///   class A { int x; };
///   class B { int y; };
///   A* a = new A();
///   B* b = static_cast<B*>(a);  // Warning: unrelated classes
/// @endcode
class AvoidUnrelatedPointerConversionCheck : public ClangTidyCheck {
public:
  /// Constructs the check with the given name and context.
  /// \param Name The name of the check as registered.
  /// \param Context The clang-tidy context for configuration.
  AvoidUnrelatedPointerConversionCheck(StringRef Name,
                                       ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidUnrelatedPointerConversionCheck() override = default;

  /// Determines if this check is language-version sensitive.
  /// \param LangOpts Language options to check.
  /// \return true if the check is applicable to C++.
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  /// Registers AST matchers for pointer cast operations.
  /// \param Finder The match finder to register matchers with.
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;

  /// Handles matched cast operations and emits diagnostics.
  /// \param Result The match result containing the matched AST node.
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDUNRELATEDPOINTERCONVERSIONCHECK_H

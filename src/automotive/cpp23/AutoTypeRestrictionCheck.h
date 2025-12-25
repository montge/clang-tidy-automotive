//===--- AutoTypeRestrictionCheck.h - clang-tidy ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AUTOTYPERESTRICTIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AUTOTYPERESTRICTIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-declaration
/// @brief Detects uses of auto where the type is not obvious from context.
///
/// The auto type specifier should only be used when the type is explicitly
/// stated in the initializer or is otherwise obvious from context. This
/// improves code readability and maintainability.
///
/// @par MISRA C++:2023 Rule 10.1.2 (Advisory)
/// The auto type specifier shall only be used when the type is explicitly
/// stated or obvious from context.
///
/// Example:
/// @code
///   // Compliant - type is explicit:
///   auto x = static_cast<int>(value);
///   auto ptr = new MyClass();
///   auto smart = std::make_unique<MyClass>();
///   auto lambda = [](int x) { return x * 2; };
///   for (auto& item : container) { }
///
///   // Non-compliant - type is not obvious:
///   auto x = getValue();        // What type does getValue return?
///   auto y = a + b;             // What is the result type?
///   auto z = 42;                // Could be int, long, unsigned...
/// @endcode
class AutoTypeRestrictionCheck : public ClangTidyCheck {
public:
  AutoTypeRestrictionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus11;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Check if the type is obvious from the initializer expression
  bool isTypeObvious(const Expr *Init) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AUTOTYPERESTRICTIONCHECK_H

//===--- CheckStreamStateCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_CHECKSTREAMSTATECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_CHECKSTREAMSTATECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// Detects use of stream objects without checking their state.
///
/// MISRA C++:2023 Rule 30.0 (partial): Standard library stream objects shall
/// not be used while in a fail state.
///
/// This check detects common violations:
/// - Multiple consecutive stream operations without state checks
/// - Using stream variables after operations without checking state
/// - Using getline() result without checking
///
/// Example:
/// @code
///   // Non-compliant: consecutive operations without checking
///   std::cin >> x >> y;
///
///   // Non-compliant: using stream after operation without check
///   std::cin >> value;
///   std::cin >> next_value;  // violation
///
///   // Compliant: check state before next use
///   std::cin >> value;
///   if (std::cin.good()) {
///     std::cin >> next_value;
///   }
/// @endcode
class CheckStreamStateCheck : public ClangTidyCheck {
public:
  CheckStreamStateCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_CHECKSTREAMSTATECHECK_H

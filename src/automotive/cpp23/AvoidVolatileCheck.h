//===--- AvoidVolatileCheck.h - clang-tidy ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVOLATILECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVOLATILECHECK_H

#include "../../ClangTidyCheck.h"
#include <string>
#include <vector>

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-types
/// @brief Detects usage of volatile qualifier except for hardware access.
///
/// The volatile keyword shall only be used for hardware access (memory-mapped
/// I/O, device registers, etc.). Using volatile for thread synchronization or
/// other purposes is incorrect and can lead to subtle bugs.
///
/// @par MISRA C++:2023 Rule 10.4.1
/// The volatile keyword shall only be used for hardware access.
/// @par Category: Advisory
///
/// Example:
/// @code
///   // Non-compliant:
///   volatile int counter = 0;  // Warning - not for hardware access
///   void func(volatile int* ptr);  // Warning
///
///   // Compliant (in allowed namespaces):
///   namespace hardware {
///     volatile uint32_t* device_register;  // OK - hardware access
///   }
/// @endcode
///
/// Options:
///   AllowedNamespaces (default: "hardware;hw;mmio") - Semicolon-separated
///                     list of namespace names where volatile is permitted
class AvoidVolatileCheck : public ClangTidyCheck {
public:
  AvoidVolatileCheck(StringRef Name, ClangTidyContext *Context);

  ~AvoidVolatileCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::vector<std::string> AllowedNamespaces;
  bool isInAllowedNamespace(const Decl *D) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_AVOIDVOLATILECHECK_H

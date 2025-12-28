//===--- BitFieldTypeRestrictionCheck.h - clang-tidy -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_BITFIELDTYPERESTRICTIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_BITFIELDTYPERESTRICTIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive::cpp23 {

/// @ingroup misra-cpp23-bitfields
/// @brief Ensures bit-fields use only approved types.
///
/// Bit-fields should only use unsigned integer types or bool to ensure
/// portable and predictable behavior. Signed bit-fields can have
/// implementation-defined behavior, and other types may not be well-supported.
///
/// @par MISRA C++:2023 Rule 12.2
/// Bit-field types shall be restricted to bool or unsigned integer types.
/// @par Category: Required
///
/// Example:
/// @code
///   // Non-compliant:
///   struct S1 {
///     int x : 5;        // Warning - signed int
///     char c : 3;       // Warning - char
///     float f : 4;      // Warning - float (not even integer)
///   };
///
///   // Compliant:
///   struct S2 {
///     unsigned int x : 5;  // OK
///     bool flag : 1;       // OK
///     uint8_t y : 3;       // OK
///   };
/// @endcode
class BitFieldTypeRestrictionCheck : public ClangTidyCheck {
public:
  BitFieldTypeRestrictionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  ~BitFieldTypeRestrictionCheck() override = default;

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isApprovedBitFieldType(QualType Type) const;
};

} // namespace clang::tidy::automotive::cpp23

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_CPP23_BITFIELDTYPERESTRICTIONCHECK_H

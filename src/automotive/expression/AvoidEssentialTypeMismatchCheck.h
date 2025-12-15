//===--- AvoidEssentialTypeMismatchCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDESSENTIALTYPEMISMATCHCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDESSENTIALTYPEMISMATCHCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects essential type mismatches in binary operations.
///
/// Implements MISRA Rule 10.4: Both operands of an operator in which the
/// usual arithmetic conversions are performed shall have the same essential
/// type category.
///
/// @par MISRA C:2025 Rule 10.4
/// Mixing signed and unsigned operands in arithmetic is error-prone.
/// @par Category: Required
///
/// Example:
/// @code
///   int32_t s = 10;
///   uint32_t u = 20;
///   int result = s + u;  // Warning: signed/unsigned mismatch
/// @endcode
class AvoidEssentialTypeMismatchCheck : public ClangTidyCheck {
public:
  AvoidEssentialTypeMismatchCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidEssentialTypeMismatchCheck() override = default;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  enum class EssentialType {
    Boolean,
    Character,
    SignedInt,
    UnsignedInt,
    FloatingPoint,
    Enum,
    Other
  };

  EssentialType getEssentialType(QualType Type) const;
  StringRef getEssentialTypeName(EssentialType ET) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDESSENTIALTYPEMISMATCHCHECK_H

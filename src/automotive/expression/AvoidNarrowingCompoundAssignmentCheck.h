//===--- AvoidNarrowingCompoundAssignmentCheck.h - clang-tidy ---*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNARROWINGCOMPOUNDASSIGNMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNARROWINGCOMPOUNDASSIGNMENTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-expression
/// @brief Detects narrowing in compound assignment expressions.
///
/// Implements MISRA Rule 10.3: The value of an expression shall not be
/// assigned to an object with a narrower essential type or of a different
/// essential type category.
///
/// @par MISRA C:2025 Rule 10.3
/// Narrowing conversions in assignments lose information and should be avoided.
/// @par Category: Required
///
/// Example:
/// @code
///   int32_t i32 = 100;
///   int16_t i16;
///   i16 = i32;           // Warning: narrowing from int32 to int16
///   uint32_t u32 = 100;
///   int32_t s32 = u32;   // Warning: unsigned to signed
/// @endcode
class AvoidNarrowingCompoundAssignmentCheck : public ClangTidyCheck {
public:
  /// Essential type categories for MISRA type analysis.
  enum class EssentialType {
    Boolean,
    Character,
    SignedInt,
    UnsignedInt,
    FloatingPoint,
    Enum,
    Other
  };

  AvoidNarrowingCompoundAssignmentCheck(StringRef Name,
                                        ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidNarrowingCompoundAssignmentCheck() override = default;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  EssentialType getEssentialType(QualType Type) const;
  unsigned getTypeWidth(QualType Type, const ASTContext &Ctx) const;
  bool isNarrowing(QualType From, QualType To, const ASTContext &Ctx) const;
  bool isDifferentCategory(EssentialType From, EssentialType To) const;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDNARROWINGCOMPOUNDASSIGNMENTCHECK_H

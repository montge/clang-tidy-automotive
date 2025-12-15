//===--- AvoidIncompatiblePointerConversionCheck.h - clang-tidy -*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPATIBLEPOINTERCONVERSIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPATIBLEPOINTERCONVERSIONCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// @ingroup misra-c25-pointer
/// @brief Detects incompatible pointer conversions.
///
/// Implements MISRA Rules 11.2, 11.5, 11.6, 11.7:
/// - 11.2: Conversion from incomplete type pointer
/// - 11.5: Conversion from void pointer to object pointer
/// - 11.6: Conversion between pointer to void and arithmetic type
/// - 11.7: Conversion between pointer to object and non-integer arithmetic
///
/// @par Category: Required/Advisory
///
/// Example:
/// @code
///   void *vp;
///   int *ip = (int *)vp;     // Warning: void* to object pointer
///   int x = (int)vp;          // Warning: pointer to integer
/// @endcode
class AvoidIncompatiblePointerConversionCheck : public ClangTidyCheck {
public:
  AvoidIncompatiblePointerConversionCheck(StringRef Name,
                                          ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  /// Destructor.
  ~AvoidIncompatiblePointerConversionCheck() override = default;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDINCOMPATIBLEPOINTERCONVERSIONCHECK_H

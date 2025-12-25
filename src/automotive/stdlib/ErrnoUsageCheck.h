//===--- ErrnoUsageCheck.h - clang-tidy -------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ERRNOUSAGECHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ERRNOUSAGECHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// MISRA C:2025 Rules 22.8 and 22.9 - errno usage requirements.
///
/// Rule 22.8: The value of errno shall be set to zero prior to a call to an
/// errno-setting function.
///
/// Rule 22.9: The value of errno shall be tested against zero after calling
/// an errno-setting function.
///
/// This check identifies calls to errno-setting functions and warns if:
/// - errno is not explicitly set to 0 before the call (22.8)
/// - errno is not tested after the call (22.9)
///
/// Note: Full dataflow analysis is complex; this check flags calls to
/// errno-setting functions as a reminder to manually verify proper usage.
class ErrnoUsageCheck : public ClangTidyCheck {
public:
  ErrnoUsageCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ERRNOUSAGECHECK_H

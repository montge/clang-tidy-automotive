//===--- ErrnoTestingCheck.h - clang-tidy -----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ERRNOTESTINGCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ERRNOTESTINGCHECK_H

#include "../../ClangTidyCheck.h"
#include "llvm/ADT/StringSet.h"

namespace clang::tidy::automotive {

/// Detects testing errno after functions that don't set it.
///
/// MISRA C:2012 Rule 22.10: The value of errno shall only be tested when
/// the last function to be called was an errno-setting-function.
///
/// Example:
/// \code
///   int x = some_function();  // Does not set errno
///   if (errno != 0) {  // Violation - testing errno after non-errno-setting
///       // ...
///   }
/// \endcode
class ErrnoTestingCheck : public ClangTidyCheck {
public:
  ErrnoTestingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  /// Returns true if the function is known to set errno.
  bool isErrnoSettingFunction(StringRef FuncName) const;

  /// Set of functions that set errno per C standard.
  static const llvm::StringSet<> ErrnoSettingFunctions;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_ERRNOTESTINGCHECK_H

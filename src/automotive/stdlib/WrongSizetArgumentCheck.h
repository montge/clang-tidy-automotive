//===--- WrongSizetArgumentCheck.h - clang-tidy -----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGSIZETARGUMENTCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGSIZETARGUMENTCHECK_H

#include "../../ClangTidyCheck.h"

namespace clang::tidy::automotive {

/// Detects invalid size_t arguments to memory and string functions.
///
/// MISRA C:2025 Rule 21.18 (Mandatory):
/// The size_t argument passed to any function in <string.h> or <stdlib.h>
/// shall have an appropriate value.
///
/// This check identifies:
/// - Size arguments that exceed the size of the source or destination buffer
/// - Negative or zero sizes where inappropriate
/// - Size computations that could overflow
///
/// Example:
/// \code
///   char buf[10];
///   memcpy(buf, src, 100);  // Warning: size exceeds buffer
/// \endcode
class WrongSizetArgumentCheck : public ClangTidyCheck {
public:
  WrongSizetArgumentCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkMemoryFunction(const CallExpr *Call, ASTContext &Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_WRONGSIZETARGUMENTCHECK_H

//===--- AvoidStdioHeaderCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDIOHEADERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDIOHEADERCHECK_H

#include "AvoidApiCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of <stdio.h> functions.
///
/// MISRA C:2025 Rule 21.6 (Required)
/// The standard library input/output routines shall not be used.
///
/// Example:
/// \code
///   #include <stdio.h>  // Violation
///   printf("hello");    // Violation
///   scanf("%d", &x);    // Violation
/// \endcode
class AvoidStdioHeaderCheck : public AvoidApiCheck {
public:
  AvoidStdioHeaderCheck(StringRef Name, ClangTidyContext *Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDSTDIOHEADERCHECK_H

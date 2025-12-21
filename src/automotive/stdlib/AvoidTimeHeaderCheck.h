//===--- AvoidTimeHeaderCheck.h - clang-tidy --------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTIMEHEADERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTIMEHEADERCHECK_H

#include "AvoidApiCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of <time.h> functions.
///
/// MISRA C:2025 Rule 21.10 (Required)
/// The standard library time and date routines shall not be used.
///
/// Example:
/// \code
///   #include <time.h>   // Violation
///   time(NULL);         // Violation
///   clock();            // Violation
/// \endcode
class AvoidTimeHeaderCheck : public AvoidApiCheck {
public:
  AvoidTimeHeaderCheck(StringRef Name, ClangTidyContext *Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTIMEHEADERCHECK_H

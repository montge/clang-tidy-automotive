//===--- AvoidTgmathHeaderCheck.h - clang-tidy ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTGMATHHEADERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTGMATHHEADERCHECK_H

#include "AvoidApiCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of <tgmath.h> header.
///
/// MISRA C:2025 Rule 21.11 (Required)
/// The <tgmath.h> header shall not be used.
///
/// Example:
/// \code
///   #include <tgmath.h>  // Violation
/// \endcode
class AvoidTgmathHeaderCheck : public AvoidApiCheck {
public:
  AvoidTgmathHeaderCheck(StringRef Name, ClangTidyContext *Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDTGMATHHEADERCHECK_H

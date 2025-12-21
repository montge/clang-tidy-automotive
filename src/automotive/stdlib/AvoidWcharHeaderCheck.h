//===--- AvoidWcharHeaderCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDWCHARHEADERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDWCHARHEADERCHECK_H

#include "AvoidApiCheck.h"

namespace clang::tidy::automotive {

/// Detects usage of <wchar.h> functions.
///
/// MISRA C:2025 Rule 21.21 (Required)
/// The <wchar.h> and <wctype.h> headers shall not be used.
///
/// Example:
/// \code
///   #include <wchar.h>   // Violation
///   wprintf(L"hello");   // Violation
/// \endcode
class AvoidWcharHeaderCheck : public AvoidApiCheck {
public:
  AvoidWcharHeaderCheck(StringRef Name, ClangTidyContext *Context);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDWCHARHEADERCHECK_H

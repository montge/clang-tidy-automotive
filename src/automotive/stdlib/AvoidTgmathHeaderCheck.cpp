//===--- AvoidTgmathHeaderCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTgmathHeaderCheck.h"

namespace clang::tidy::automotive {

// MISRA C:2012 Rule 21.11: The standard header file <tgmath.h> shall not be used
// We ONLY check for header inclusion - standard math functions from <math.h>
// or <cmath> are perfectly fine. The issue is the type-generic macros in tgmath.h
// that make code harder to analyze statically.

AvoidTgmathHeaderCheck::AvoidTgmathHeaderCheck(StringRef Name,
                                               ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "tgmath.h", {}) {}

} // namespace clang::tidy::automotive

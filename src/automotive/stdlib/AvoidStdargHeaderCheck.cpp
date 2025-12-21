//===--- AvoidStdargHeaderCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStdargHeaderCheck.h"

namespace clang::tidy::automotive {

static const StringRef AvoidFunctions[] = {"va_start", "va_arg", "va_end",
                                           "va_copy"};

AvoidStdargHeaderCheck::AvoidStdargHeaderCheck(StringRef Name,
                                               ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "stdarg.h", AvoidFunctions) {}

} // namespace clang::tidy::automotive

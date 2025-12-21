//===--- AvoidCtypeHeaderCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCtypeHeaderCheck.h"

namespace clang::tidy::automotive {

static const StringRef AvoidFunctions[] = {
    "isalnum",  "isalpha", "isblank", "iscntrl", "isdigit",
    "isgraph",  "islower", "isprint", "ispunct", "isspace",
    "isupper",  "isxdigit", "tolower", "toupper"};

AvoidCtypeHeaderCheck::AvoidCtypeHeaderCheck(StringRef Name,
                                             ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "ctype.h", AvoidFunctions) {}

} // namespace clang::tidy::automotive

//===--- AvoidTimeHeaderCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTimeHeaderCheck.h"

namespace clang::tidy::automotive {

static const StringRef AvoidFunctions[] = {
    "clock",     "difftime", "mktime", "time",    "asctime",
    "ctime",     "gmtime",   "localtime", "strftime", "timespec_get"};

AvoidTimeHeaderCheck::AvoidTimeHeaderCheck(StringRef Name,
                                           ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "time.h", AvoidFunctions) {}

} // namespace clang::tidy::automotive

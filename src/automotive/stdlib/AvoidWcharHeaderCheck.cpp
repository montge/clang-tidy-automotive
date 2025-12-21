//===--- AvoidWcharHeaderCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidWcharHeaderCheck.h"

namespace clang::tidy::automotive {

static const StringRef AvoidFunctions[] = {
    "wprintf",   "fwprintf", "swprintf", "vwprintf", "vfwprintf", "vswprintf",
    "wscanf",    "fwscanf",  "swscanf",  "vwscanf",  "vfwscanf",  "vswscanf",
    "fgetwc",    "fgetws",   "fputwc",   "fputws",   "getwc",     "getwchar",
    "putwc",     "putwchar", "ungetwc",  "wcstod",   "wcstof",    "wcstol",
    "wcstold",   "wcstoll",  "wcstoul",  "wcstoull", "wcscpy",    "wcsncpy",
    "wcscat",    "wcsncat",  "wcscmp",   "wcsncmp",  "wcscoll",   "wcsxfrm",
    "wcschr",    "wcsrchr",  "wcspbrk",  "wcsstr",   "wcstok",    "wcslen",
    "wmemcpy",   "wmemmove", "wmemcmp",  "wmemchr",  "wmemset",   "wcsftime",
    "btowc",     "wctob",    "mbsinit",  "mbrlen",   "mbrtowc",   "wcrtomb",
    "mbsrtowcs", "wcsrtombs"};

AvoidWcharHeaderCheck::AvoidWcharHeaderCheck(StringRef Name,
                                             ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "wchar.h", AvoidFunctions) {}

} // namespace clang::tidy::automotive

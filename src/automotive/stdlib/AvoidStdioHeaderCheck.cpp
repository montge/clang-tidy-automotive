//===--- AvoidStdioHeaderCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStdioHeaderCheck.h"

namespace clang::tidy::automotive {

static const StringRef AvoidFunctions[] = {
    "printf",   "fprintf",   "sprintf", "snprintf", "vprintf", "vfprintf",
    "vsprintf", "vsnprintf", "scanf",   "fscanf",   "sscanf",  "vscanf",
    "vfscanf",  "vsscanf",   "fopen",   "fclose",   "fread",   "fwrite",
    "fflush",   "fgetc",     "fgets",   "fputc",    "fputs",   "getc",
    "getchar",  "gets",      "putc",    "putchar",  "puts",    "ungetc",
    "fseek",    "ftell",     "rewind",  "fgetpos",  "fsetpos", "clearerr",
    "feof",     "ferror",    "perror",  "remove",   "rename",  "tmpfile",
    "tmpnam",   "freopen",   "setbuf",  "setvbuf"};

AvoidStdioHeaderCheck::AvoidStdioHeaderCheck(StringRef Name,
                                             ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "stdio.h", AvoidFunctions) {}

} // namespace clang::tidy::automotive

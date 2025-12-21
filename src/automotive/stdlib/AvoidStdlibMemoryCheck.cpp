//===--- AvoidStdlibMemoryCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStdlibMemoryCheck.h"

namespace clang::tidy::automotive {

static const StringRef AvoidFunctions[] = {"malloc", "calloc", "realloc",
                                           "free", "aligned_alloc"};

AvoidStdlibMemoryCheck::AvoidStdlibMemoryCheck(StringRef Name,
                                               ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "stdlib.h", AvoidFunctions) {}

} // namespace clang::tidy::automotive

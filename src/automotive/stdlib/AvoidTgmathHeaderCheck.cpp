//===--- AvoidTgmathHeaderCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTgmathHeaderCheck.h"

namespace clang::tidy::automotive {

// tgmath.h defines type-generic macros that expand to functions from math.h
// We primarily check for header inclusion, but also flag some common usages
static const StringRef AvoidFunctions[] = {
    "acos", "asin",  "atan", "atan2", "ceil",  "cos", "cosh",  "exp",
    "fabs", "floor", "fmod", "frexp", "ldexp", "log", "log10", "modf",
    "pow",  "sin",   "sinh", "sqrt",  "tan",   "tanh"};

AvoidTgmathHeaderCheck::AvoidTgmathHeaderCheck(StringRef Name,
                                               ClangTidyContext *Context)
    : AvoidApiCheck(Name, Context, "tgmath.h", AvoidFunctions) {}

} // namespace clang::tidy::automotive

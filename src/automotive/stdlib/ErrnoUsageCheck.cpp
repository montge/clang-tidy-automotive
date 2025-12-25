//===--- ErrnoUsageCheck.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ErrnoUsageCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
// Functions documented to set errno on error
// From C11/C23 standard library and POSIX
const char *ErrnoSettingFunctions[] = {
    // Math functions (many set errno on domain/range errors)
    "acos",   "acosf",   "acosl",   "asin",    "asinf",   "asinl",
    "atan2",  "atan2f",  "atan2l",  "cosh",    "coshf",   "coshl",
    "exp",    "expf",    "expl",    "exp2",    "exp2f",   "exp2l",
    "expm1",  "expm1f",  "expm1l",  "fmod",    "fmodf",   "fmodl",
    "hypot",  "hypotf",  "hypotl",  "ldexp",   "ldexpf",  "ldexpl",
    "lgamma", "lgammaf", "lgammal", "log",     "logf",    "logl",
    "log10",  "log10f",  "log10l",  "log1p",   "log1pf",  "log1pl",
    "log2",   "log2f",   "log2l",   "pow",     "powf",    "powl",
    "sinh",   "sinhf",   "sinhl",   "sqrt",    "sqrtf",   "sqrtl",
    "tgamma", "tgammaf", "tgammal", "y0",      "y0f",     "y0l",
    "y1",     "y1f",     "y1l",     "yn",      "ynf",     "ynl",
    "j0",     "j0f",     "j0l",     "j1",      "j1f",     "j1l",
    "jn",     "jnf",     "jnl",     "scalbn",  "scalbnf", "scalbnl",
    "scalbln","scalblnf","scalblnl",

    // String to number conversions
    "strtol",  "strtoll", "strtoul", "strtoull",
    "strtof",  "strtod",  "strtold",
    "strtoimax", "strtoumax",
    "wcstol",  "wcstoll", "wcstoul", "wcstoull",
    "wcstof",  "wcstod",  "wcstold",
    "wcstoimax", "wcstoumax",

    // I/O functions
    "fopen",   "freopen", "fclose",  "fflush",
    "fgetc",   "fgets",   "fputc",   "fputs",
    "getc",    "putc",    "ungetc",
    "fread",   "fwrite",
    "fseek",   "ftell",   "fgetpos", "fsetpos",
    "feof",    "ferror",
    "printf",  "fprintf", "sprintf", "snprintf",
    "scanf",   "fscanf",  "sscanf",
    "vprintf", "vfprintf","vsprintf","vsnprintf",
    "vscanf",  "vfscanf", "vsscanf",
    "remove",  "rename",  "tmpfile", "tmpnam",

    // Memory allocation
    "malloc",  "calloc",  "realloc", "aligned_alloc",

    // Character classification/conversion
    "mblen",   "mbtowc",  "wctomb",
    "mbstowcs","wcstombs",
    "mbrtowc", "wcrtomb", "mbsrtowcs", "wcsrtombs",

    // Time functions
    "mktime",  "time",    "clock",

    // Signal handling
    "signal",  "raise",

    // POSIX functions (common ones)
    "open",    "close",   "read",    "write",
    "lseek",   "stat",    "fstat",   "lstat",
    "mkdir",   "rmdir",   "unlink",  "link",
    "chmod",   "chown",   "access",
    "opendir", "readdir", "closedir",
    "fork",    "exec",    "execv",   "execve",
    "wait",    "waitpid",
    "socket",  "connect", "bind",    "listen", "accept",
    "send",    "recv",    "sendto",  "recvfrom",
    "pthread_create", "pthread_join", "pthread_mutex_init",
};

bool isErrnoSettingFunction(StringRef Name) {
  for (const char *Func : ErrnoSettingFunctions) {
    if (Name == Func)
      return true;
  }
  return false;
}
} // namespace

void ErrnoUsageCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to errno-setting functions
  Finder->addMatcher(callExpr(callee(functionDecl().bind("func"))).bind("call"),
                     this);
}

void ErrnoUsageCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

  if (!Call || !Func)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  StringRef FuncName = Func->getName();

  // Check if this is an errno-setting function
  if (!isErrnoSettingFunction(FuncName))
    return;

  // Flag the call - full dataflow analysis to verify errno usage
  // is complex, so we provide a reminder for manual review
  diag(Call->getBeginLoc(),
       "call to errno-setting function '%0'; ensure errno is set to 0 "
       "before the call and tested after (Rules 22.8, 22.9)")
      << FuncName;
}

} // namespace clang::tidy::automotive

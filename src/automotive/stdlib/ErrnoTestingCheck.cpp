//===--- ErrnoTestingCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ErrnoTestingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

// Functions that are specified to set errno in the C standard
// This list includes C99/C11/C17 errno-setting functions
const llvm::StringSet<> ErrnoTestingCheck::ErrnoSettingFunctions = {
    // <math.h> functions
    "acos",
    "acosf",
    "acosl",
    "asin",
    "asinf",
    "asinl",
    "atan2",
    "atan2f",
    "atan2l",
    "atanh",
    "atanhf",
    "atanhl",
    "cosh",
    "coshf",
    "coshl",
    "exp",
    "expf",
    "expl",
    "exp2",
    "exp2f",
    "exp2l",
    "expm1",
    "expm1f",
    "expm1l",
    "fdim",
    "fdimf",
    "fdiml",
    "fma",
    "fmaf",
    "fmal",
    "hypot",
    "hypotf",
    "hypotl",
    "ilogb",
    "ilogbf",
    "ilogbl",
    "ldexp",
    "ldexpf",
    "ldexpl",
    "lgamma",
    "lgammaf",
    "lgammal",
    "llrint",
    "llrintf",
    "llrintl",
    "llround",
    "llroundf",
    "llroundl",
    "log",
    "logf",
    "logl",
    "log10",
    "log10f",
    "log10l",
    "log1p",
    "log1pf",
    "log1pl",
    "log2",
    "log2f",
    "log2l",
    "logb",
    "logbf",
    "logbl",
    "lrint",
    "lrintf",
    "lrintl",
    "lround",
    "lroundf",
    "lroundl",
    "nextafter",
    "nextafterf",
    "nextafterl",
    "nexttoward",
    "nexttowardf",
    "nexttowardl",
    "pow",
    "powf",
    "powl",
    "remainder",
    "remainderf",
    "remainderl",
    "remquo",
    "remquof",
    "remquol",
    "rint",
    "rintf",
    "rintl",
    "scalbln",
    "scalblnf",
    "scalblnl",
    "scalbn",
    "scalbnf",
    "scalbnl",
    "sinh",
    "sinhf",
    "sinhl",
    "sqrt",
    "sqrtf",
    "sqrtl",
    "tgamma",
    "tgammaf",
    "tgammal",
    "y0",
    "y1",
    "yn",
    // <stdlib.h> functions
    "strtol",
    "strtoll",
    "strtoul",
    "strtoull",
    "strtof",
    "strtod",
    "strtold",
    "strtoimax",
    "strtoumax",
    "wcstol",
    "wcstoll",
    "wcstoul",
    "wcstoull",
    "wcstof",
    "wcstod",
    "wcstold",
    "wcstoimax",
    "wcstoumax",
    // <stdio.h> functions
    "fopen",
    "freopen",
    "fclose",
    "fflush",
    "fread",
    "fwrite",
    "fgetc",
    "fgets",
    "fputc",
    "fputs",
    "getc",
    "putc",
    "ungetc",
    "fseek",
    "ftell",
    "fsetpos",
    "fgetpos",
    "rewind",
    "fprintf",
    "fscanf",
    "printf",
    "scanf",
    "snprintf",
    "sprintf",
    "sscanf",
    "vfprintf",
    "vfscanf",
    "vprintf",
    "vscanf",
    "vsnprintf",
    "vsprintf",
    "vsscanf",
    "fgetwc",
    "fgetws",
    "fputwc",
    "fputws",
    "getwc",
    "putwc",
    "ungetwc",
    "fwprintf",
    "fwscanf",
    "wprintf",
    "wscanf",
    "swprintf",
    "swscanf",
    "vfwprintf",
    "vfwscanf",
    "vwprintf",
    "vwscanf",
    "vswprintf",
    "vswscanf",
    "remove",
    "rename",
    "tmpfile",
    "tmpnam",
    // <string.h> functions
    "strerror",
    // <wchar.h> functions
    "mbrtowc",
    "wcrtomb",
    "mbsrtowcs",
    "wcsrtombs",
    // <signal.h> functions
    "signal",
    // <time.h> functions
    "mktime",
    "time",
    "clock",
    // POSIX functions (commonly used)
    "open",
    "close",
    "read",
    "write",
    "lseek",
    "stat",
    "fstat",
    "lstat",
    "mkdir",
    "rmdir",
    "unlink",
    "link",
    "symlink",
    "readlink",
    "chmod",
    "chown",
    "access",
    "dup",
    "dup2",
    "pipe",
    "socket",
    "bind",
    "listen",
    "accept",
    "connect",
    "send",
    "recv",
    "sendto",
    "recvfrom",
    "setsockopt",
    "getsockopt",
    "getaddrinfo",
    "getnameinfo",
    "pthread_create",
    "pthread_join",
    "pthread_mutex_init",
    "pthread_mutex_lock",
    "pthread_mutex_unlock",
    "pthread_cond_init",
    "pthread_cond_wait",
    "pthread_cond_signal",
    "sem_open",
    "sem_close",
    "sem_wait",
    "sem_post",
    "mmap",
    "munmap",
    "mprotect",
    "msync",
    "fork",
    "exec",
    "execve",
    "execvp",
    "wait",
    "waitpid",
    "kill",
    "raise",
    "alarm",
    "sleep",
    "usleep",
    "nanosleep",
    "getcwd",
    "chdir",
    "opendir",
    "readdir",
    "closedir",
    "getenv",
    "setenv",
    "unsetenv",
    "system",
    "popen",
    "pclose",
    "dlopen",
    "dlsym",
    "dlclose",
    // Memory allocation
    "malloc",
    "calloc",
    "realloc",
    "aligned_alloc",
    "posix_memalign",
};

void ErrnoTestingCheck::registerMatchers(MatchFinder *Finder) {
  // Match expressions that reference errno in a comparison
  // errno is typically a macro that expands to something like
  // (*__errno_location()) We look for DeclRefExpr to a variable named "errno"
  // or calls to __errno_location
  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()),
          anyOf(hasOperatorName("=="), hasOperatorName("!=")),
          anyOf(
              hasLHS(ignoringParenImpCasts(
                  declRefExpr(to(varDecl(hasName("errno")))))),
              hasRHS(ignoringParenImpCasts(
                  declRefExpr(to(varDecl(hasName("errno")))))),
              // Also match errno macro expansion patterns
              hasLHS(ignoringParenImpCasts(unaryOperator(
                  hasOperatorName("*"),
                  hasUnaryOperand(callExpr(callee(functionDecl(anyOf(
                      hasName("__errno_location"), hasName("__errno"))))))))),
              hasRHS(ignoringParenImpCasts(unaryOperator(
                  hasOperatorName("*"),
                  hasUnaryOperand(callExpr(callee(functionDecl(anyOf(
                      hasName("__errno_location"), hasName("__errno")))))))))))
          .bind("errnoTest"),
      this);
}

bool ErrnoTestingCheck::isErrnoSettingFunction(StringRef FuncName) const {
  return ErrnoSettingFunctions.contains(FuncName);
}

void ErrnoTestingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ErrnoTest = Result.Nodes.getNodeAs<BinaryOperator>("errnoTest");
  if (!ErrnoTest)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(ErrnoTest->getBeginLoc()))
    return;

  // Find the compound statement containing this errno test
  auto Parents = Result.Context->getParents(*ErrnoTest);
  const Stmt *Current = ErrnoTest;

  // Walk up to find the enclosing compound statement or condition
  while (!Parents.empty()) {
    const Stmt *Parent = Parents[0].get<Stmt>();
    if (!Parent)
      break;

    // If we're in an if/while/for condition, look at statements before this
    if (const auto *IfS = dyn_cast<IfStmt>(Parent)) {
      // Look for function calls in the same block before this if statement
      auto IfParents = Result.Context->getParents(*IfS);
      if (!IfParents.empty()) {
        if (const auto *Compound = IfParents[0].get<CompoundStmt>()) {
          // Find the if statement in the compound and look for calls before it
          const CallExpr *LastCall = nullptr;
          for (const Stmt *S : Compound->body()) {
            if (S == IfS)
              break;
            // Look for call expressions in this statement
            if (const auto *Call = dyn_cast<CallExpr>(S)) {
              LastCall = Call;
            } else if (const auto *DeclS = dyn_cast<DeclStmt>(S)) {
              // Check for calls in variable initializers
              for (const Decl *D : DeclS->decls()) {
                if (const auto *VD = dyn_cast<VarDecl>(D)) {
                  if (const Expr *Init = VD->getInit()) {
                    if (const auto *Call =
                            dyn_cast<CallExpr>(Init->IgnoreParenImpCasts())) {
                      LastCall = Call;
                    }
                  }
                }
              }
            } else if (const auto *ExprS = dyn_cast<Expr>(S)) {
              // Look for calls in expression statements
              if (const auto *Call =
                      dyn_cast<CallExpr>(ExprS->IgnoreParenImpCasts())) {
                LastCall = Call;
              }
            }
          }

          if (LastCall) {
            if (const FunctionDecl *FD = LastCall->getDirectCallee()) {
              StringRef FuncName = FD->getName();
              if (!isErrnoSettingFunction(FuncName)) {
                diag(ErrnoTest->getBeginLoc(),
                     "errno tested after calling '%0' which is not an "
                     "errno-setting function")
                    << FuncName;
                diag(LastCall->getBeginLoc(), "last function call was here",
                     DiagnosticIDs::Note);
                return;
              }
            }
          } else {
            // No function call found before errno test
            diag(
                ErrnoTest->getBeginLoc(),
                "errno tested without a preceding errno-setting function call");
            return;
          }
        }
      }
      break;
    }

    Parents = Result.Context->getParents(*Parent);
    Current = Parent;
  }
}

} // namespace clang::tidy::automotive

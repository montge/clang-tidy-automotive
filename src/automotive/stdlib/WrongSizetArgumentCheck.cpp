//===--- WrongSizetArgumentCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "WrongSizetArgumentCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void WrongSizetArgumentCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to memory and string functions
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName(
                   "memcpy", "memmove", "memset", "memcmp", "strcpy", "strncpy",
                   "strcat", "strncat", "strcmp", "strncmp", "strlen"))))
          .bind("call"),
      this);
}

void WrongSizetArgumentCheck::checkMemoryFunction(const CallExpr *Call,
                                                   ASTContext &Context) {
  const auto *Callee = Call->getDirectCallee();
  if (!Callee)
    return;

  StringRef FuncName = Callee->getName();

  // Determine which argument is the size argument
  unsigned SizeArgIdx = 0;
  if (FuncName == "memcpy" || FuncName == "memmove" || FuncName == "memcmp")
    SizeArgIdx = 2;
  else if (FuncName == "memset")
    SizeArgIdx = 2;
  else if (FuncName == "strncpy" || FuncName == "strncat" ||
           FuncName == "strncmp")
    SizeArgIdx = 2;
  else
    return; // No size argument to check

  if (SizeArgIdx >= Call->getNumArgs())
    return;

  const Expr *SizeArg = Call->getArg(SizeArgIdx);
  if (!SizeArg)
    return;

  // Try to evaluate the size as a constant
  Expr::EvalResult EvalResult;
  if (SizeArg->EvaluateAsInt(EvalResult, Context)) {
    llvm::APSInt SizeValue = EvalResult.Val.getInt();

    // Check for negative size
    if (SizeValue.isNegative()) {
      diag(SizeArg->getBeginLoc(), "size_t argument shall not be negative");
      return;
    }

    // Check if size is zero for functions where it doesn't make sense
    if (SizeValue.isZero() && (FuncName == "memcpy" || FuncName == "memmove" ||
                                FuncName == "strncpy")) {
      diag(SizeArg->getBeginLoc(),
           "size_t argument of zero has no effect for function '%0'")
          << FuncName;
    }

    // For memcpy, memmove, memset: check if size exceeds destination buffer
    if (FuncName == "memcpy" || FuncName == "memmove" ||
        FuncName == "memset") {
      const Expr *DestArg = Call->getArg(0);
      if (DestArg) {
        DestArg = DestArg->IgnoreParenImpCasts();

        // If destination is a known array, check bounds
        if (const auto *DRE = dyn_cast<DeclRefExpr>(DestArg)) {
          if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
            QualType VarType = VD->getType();
            if (const auto *CAT = dyn_cast<ConstantArrayType>(VarType)) {
              uint64_t ArraySize = CAT->getSize().getZExtValue();
              uint64_t ElementSize =
                  Context.getTypeSize(CAT->getElementType()) / 8;
              uint64_t TotalSize = ArraySize * ElementSize;

              if (SizeValue.getZExtValue() > TotalSize) {
                diag(SizeArg->getBeginLoc(),
                     "size_t argument exceeds destination buffer size of %0 "
                     "bytes")
                    << static_cast<unsigned>(TotalSize);
              }
            }
          }
        }
      }
    }
  }
}

void WrongSizetArgumentCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  if (!Call)
    return;

  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  checkMemoryFunction(Call, *Result.Context);
}

} // namespace clang::tidy::automotive

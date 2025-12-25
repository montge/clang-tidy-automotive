//===--- TgmathConsistentTypeCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TgmathConsistentTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Multi-argument tgmath functions where all operands should have same type
// Maps function name pattern to indices of operands to check
struct TgmathFuncInfo {
  const char *Pattern;
  std::vector<unsigned> OperandIndices;
};

// Functions with 2 operands to check (indices 0 and 1)
const char *TwoOpFuncs[] = {"atan2", "copysign", "fdim",  "fmax",
                            "fmin",  "fmod",     "hypot", "nextafter",
                            "pow",   "remainder"};

// fma has 3 operands to check (indices 0, 1, 2)
// frexp: only index 0 matters (index 1 is output pointer)
// ldexp, scalbn, scalbln: only index 0 matters (index 1 is integer)
// remquo: indices 0 and 1 (index 2 is output pointer)
// nexttoward: only index 0 matters (index 1 is always long double)

bool isTwoOpFunc(StringRef Name) {
  // Strip f/l suffix for float/long double variants
  StringRef BaseName = Name;
  if (BaseName.ends_with("f") || BaseName.ends_with("l"))
    BaseName = BaseName.drop_back(1);

  for (const char *Func : TwoOpFuncs) {
    if (BaseName == Func)
      return true;
  }
  return false;
}

bool isFmaFunc(StringRef Name) {
  return Name == "fma" || Name == "fmaf" || Name == "fmal";
}

bool isRemquoFunc(StringRef Name) {
  return Name == "remquo" || Name == "remquof" || Name == "remquol";
}

} // anonymous namespace

void TgmathConsistentTypeCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to math functions
  Finder->addMatcher(
      callExpr(
          callee(functionDecl(hasAnyName(
              // Two-operand functions (all variants)
              "atan2", "atan2f", "atan2l", "copysign", "copysignf", "copysignl",
              "fdim", "fdimf", "fdiml", "fmax", "fmaxf", "fmaxl", "fmin",
              "fminf", "fminl", "fmod", "fmodf", "fmodl", "hypot", "hypotf",
              "hypotl", "nextafter", "nextafterf", "nextafterl", "pow", "powf",
              "powl", "remainder", "remainderf", "remainderl",
              // Three-operand functions
              "fma", "fmaf", "fmal", "remquo", "remquof", "remquol"))))
          .bind("call"),
      this);
}

void TgmathConsistentTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  if (!Call)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  const FunctionDecl *FD = Call->getDirectCallee();
  if (!FD)
    return;

  StringRef FuncName = FD->getName();
  std::vector<unsigned> IndicesToCheck;

  if (isTwoOpFunc(FuncName)) {
    IndicesToCheck = {0, 1};
  } else if (isFmaFunc(FuncName)) {
    IndicesToCheck = {0, 1, 2};
  } else if (isRemquoFunc(FuncName)) {
    IndicesToCheck = {0, 1};
  } else {
    return;
  }

  // Check that all operands have the same type (before implicit conversions)
  if (IndicesToCheck.size() < 2)
    return;

  // Get the first operand's type as reference
  if (IndicesToCheck[0] >= Call->getNumArgs())
    return;

  const Expr *FirstArg = Call->getArg(IndicesToCheck[0])->IgnoreParenImpCasts();
  QualType FirstType = FirstArg->getType();

  // Skip if not a floating point type
  if (!FirstType->isRealFloatingType())
    return;

  // Check remaining operands
  for (size_t I = 1; I < IndicesToCheck.size(); ++I) {
    unsigned Idx = IndicesToCheck[I];
    if (Idx >= Call->getNumArgs())
      continue;

    const Expr *Arg = Call->getArg(Idx)->IgnoreParenImpCasts();
    QualType ArgType = Arg->getType();

    // Integer arguments are promoted, so we only check floating types
    if (!ArgType->isRealFloatingType())
      continue;

    // Compare canonical types
    if (FirstType.getCanonicalType() != ArgType.getCanonicalType()) {
      diag(Call->getBeginLoc(),
           "arguments to '%0' have inconsistent types: '%1' and '%2'; "
           "all operands should have the same floating-point type")
          << FuncName << FirstType.getAsString() << ArgType.getAsString();
      return;
    }
  }
}

} // namespace clang::tidy::automotive

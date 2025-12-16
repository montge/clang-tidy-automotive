//===--- AvoidDesignatedInitializerBoundsCheck.cpp - clang-tidy -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidDesignatedInitializerBoundsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidDesignatedInitializerBoundsCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match designated initializer expressions
  Finder->addMatcher(designatedInitExpr().bind("designated"), this);
}

void AvoidDesignatedInitializerBoundsCheck::checkArrayDesignator(
    const DesignatedInitExpr::Designator &D, const DesignatedInitExpr *DIE,
    const MatchFinder::MatchResult &Result) {
  const Expr *IndexExpr = DIE->getArrayIndex(D);
  if (!IndexExpr)
    return;

  auto IndexValue = IndexExpr->getIntegerConstantExpr(*Result.Context);
  if (!IndexValue)
    return;

  int64_t Index = IndexValue->getSExtValue();
  if (Index < 0) {
    diag(D.getLBracketLoc(),
         "designated initializer has negative array index %0")
        << static_cast<int>(Index);
  }
}

void AvoidDesignatedInitializerBoundsCheck::checkArrayRangeDesignator(
    const DesignatedInitExpr::Designator &D, const DesignatedInitExpr *DIE,
    const MatchFinder::MatchResult &Result) {
  const Expr *StartExpr = DIE->getArrayRangeStart(D);
  const Expr *EndExpr = DIE->getArrayRangeEnd(D);
  if (!StartExpr || !EndExpr)
    return;

  auto StartValue = StartExpr->getIntegerConstantExpr(*Result.Context);
  auto EndValue = EndExpr->getIntegerConstantExpr(*Result.Context);
  if (!StartValue || !EndValue)
    return;

  int64_t Start = StartValue->getSExtValue();
  int64_t End = EndValue->getSExtValue();

  if (Start < 0 || End < 0) {
    diag(D.getLBracketLoc(),
         "designated initializer array range has negative index");
    return;
  }

  if (Start > End) {
    diag(D.getLBracketLoc(),
         "designated initializer array range has start index %0 "
         "greater than end index %1")
        << static_cast<int>(Start) << static_cast<int>(End);
  }
}

void AvoidDesignatedInitializerBoundsCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *DIE = Result.Nodes.getNodeAs<DesignatedInitExpr>("designated");
  if (!DIE)
    return;

  if (Result.SourceManager->isInSystemHeader(DIE->getBeginLoc()))
    return;

  for (const auto &D : DIE->designators()) {
    if (D.isArrayDesignator())
      checkArrayDesignator(D, DIE, Result);
    else if (D.isArrayRangeDesignator())
      checkArrayRangeDesignator(D, DIE, Result);
  }
}

} // namespace clang::tidy::automotive

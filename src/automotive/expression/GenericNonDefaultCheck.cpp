//===--- GenericNonDefaultCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericNonDefaultCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericNonDefaultCheck::registerMatchers(MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("genericSelection"), this);
}

void GenericNonDefaultCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  unsigned NumAssocs = GenericSelection->getNumAssocs();

  // No associations at all - edge case, should not happen in valid code
  if (NumAssocs == 0)
    return;

  // Count non-default associations
  // A default association has a null TypeSourceInfo
  unsigned NonDefaultCount = 0;
  for (unsigned I = 0; I < NumAssocs; ++I) {
    auto Assoc = GenericSelection->getAssociation(I);
    if (Assoc.getTypeSourceInfo()) {
      ++NonDefaultCount;
    }
  }

  // If there are no non-default associations, this is a violation
  if (NonDefaultCount == 0) {
    diag(GenericSelection->getBeginLoc(),
         "generic selection should contain at least one non-default "
         "association");
  }
}

} // namespace clang::tidy::automotive

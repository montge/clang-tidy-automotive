//===--- GenericDefaultPositionCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericDefaultPositionCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericDefaultPositionCheck::registerMatchers(MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("genericSelection"), this);
}

void GenericDefaultPositionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  unsigned NumAssocs = GenericSelection->getNumAssocs();

  // No violation if there are 0 or 1 associations (default is automatically
  // first/last)
  if (NumAssocs <= 1)
    return;

  // Find the default association (if any)
  // The default association has a null TypeSourceInfo
  int DefaultIndex = -1;
  for (unsigned I = 0; I < NumAssocs; ++I) {
    auto Assoc = GenericSelection->getAssociation(I);
    if (!Assoc.getTypeSourceInfo()) {
      DefaultIndex = static_cast<int>(I);
      break;
    }
  }

  // No default association found - no violation
  if (DefaultIndex == -1)
    return;

  // Check if default is first (index 0) or last (index NumAssocs - 1)
  unsigned LastIndex = NumAssocs - 1;
  if (DefaultIndex != 0 && DefaultIndex != static_cast<int>(LastIndex)) {
    // Default is in the middle - this is a violation
    auto DefaultAssoc = GenericSelection->getAssociation(DefaultIndex);
    diag(
        DefaultAssoc.getAssociationExpr()->getBeginLoc(),
        "default association in generic selection should appear first or last, "
        "not in the middle");
  }
}

} // namespace clang::tidy::automotive

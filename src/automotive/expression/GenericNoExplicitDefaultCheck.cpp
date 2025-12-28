//===--- GenericNoExplicitDefaultCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericNoExplicitDefaultCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericNoExplicitDefaultCheck::registerMatchers(MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("genericSelection"), this);
}

void GenericNoExplicitDefaultCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  unsigned NumAssocs = GenericSelection->getNumAssocs();

  // Check each association for default
  for (unsigned I = 0; I < NumAssocs; ++I) {
    auto Assoc = GenericSelection->getAssociation(I);
    // A default association has a null TypeSourceInfo
    if (!Assoc.getTypeSourceInfo()) {
      diag(GenericSelection->getBeginLoc(),
           "generic selection should not have an explicit default association");
      return;
    }
  }
}

} // namespace clang::tidy::automotive

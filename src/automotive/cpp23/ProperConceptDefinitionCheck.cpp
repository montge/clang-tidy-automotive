//===--- ProperConceptDefinitionCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ProperConceptDefinitionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ProperConceptDefinitionCheck::registerMatchers(MatchFinder *Finder) {
  // Match concept declarations
  Finder->addMatcher(
      conceptDecl(unless(isExpansionInSystemHeader())).bind("concept"), this);
}

bool ProperConceptDefinitionCheck::isTrivialConstraint(const Expr *E) const {
  if (!E)
    return false;

  // Strip any implicit casts
  E = E->IgnoreParenImpCasts();

  // Check for boolean literals (true/false)
  if (isa<CXXBoolLiteralExpr>(E))
    return true;

  // Check for integral constant that evaluates to true/false
  if (isa<IntegerLiteral>(E))
    return true;

  return false;
}

void ProperConceptDefinitionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *CD = Result.Nodes.getNodeAs<ConceptDecl>("concept");
  if (!CD)
    return;

  const Expr *ConstraintExpr = CD->getConstraintExpr();
  if (!ConstraintExpr)
    return;

  // Check for trivially satisfied concepts (e.g., concept X = true;)
  if (isTrivialConstraint(ConstraintExpr)) {
    diag(CD->getLocation(),
         "concept %0 is trivially satisfied; concepts should express "
         "meaningful constraints")
        << CD;
    return;
  }
}

} // namespace clang::tidy::automotive

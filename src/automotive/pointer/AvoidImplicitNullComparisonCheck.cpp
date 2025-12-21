//===--- AvoidImplicitNullComparisonCheck.cpp - clang-tidy ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidImplicitNullComparisonCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidImplicitNullComparisonCheck::registerMatchers(MatchFinder *Finder) {
  // Match pointer expressions used directly in conditions
  // (not as part of explicit comparisons)
  Finder->addMatcher(
      implicitCastExpr(
          hasCastKind(CK_PointerToBoolean),
          hasSourceExpression(expr(hasType(pointerType())).bind("pointer")))
          .bind("cast"),
      this);
}

void AvoidImplicitNullComparisonCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<ImplicitCastExpr>("cast");
  if (!Cast)
    return;

  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  // Check if this cast is part of an explicit comparison (which is OK)
  // We look for the parent to see if it's already a comparison
  const auto &Parents = Result.Context->getParents(*Cast);
  for (const auto &Parent : Parents) {
    if (const auto *BO = Parent.get<BinaryOperator>()) {
      // If parent is == or !=, this is an explicit comparison
      if (BO->isEqualityOp())
        return;
    }
  }

  diag(Cast->getBeginLoc(), "pointer shall be explicitly compared to NULL");
}

} // namespace clang::tidy::automotive

//===--- GenericAppropriateTypeCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericAppropriateTypeCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericAppropriateTypeCheck::registerMatchers(MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("generic"), this);
}

void GenericAppropriateTypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GS = Result.Nodes.getNodeAs<GenericSelectionExpr>("generic");
  if (!GS)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(GS->getBeginLoc()))
    return;

  // Check each association's type
  for (unsigned I = 0; I < GS->getNumAssocs(); ++I) {
    auto Assoc = GS->getAssociation(I);

    // Skip default association (no type)
    if (!Assoc.getTypeSourceInfo())
      continue;

    QualType AssocType = Assoc.getType();
    SourceLocation TypeLoc = Assoc.getTypeSourceInfo()->getTypeLoc().getBeginLoc();

    // Check for void type
    if (AssocType->isVoidType()) {
      diag(TypeLoc, "generic association type 'void' is not an object type; "
                    "use a complete object type");
      continue;
    }

    // Check for function type (not pointer-to-function)
    if (AssocType->isFunctionType()) {
      diag(TypeLoc, "generic association has function type %0; "
                    "use pointer-to-function type instead")
          << AssocType;
      continue;
    }

    // Check for incomplete types (except void which is handled above)
    if (AssocType->isIncompleteType() && !AssocType->isVoidType()) {
      diag(TypeLoc, "generic association has incomplete type %0; "
                    "use a complete type")
          << AssocType;
      continue;
    }

    // Check for variably modified types (VLAs) - these cannot appear in
    // generic associations
    if (AssocType->isVariablyModifiedType()) {
      diag(TypeLoc,
           "generic association has variably modified type %0; "
           "use a type with constant size")
          << AssocType;
      continue;
    }
  }
}

} // namespace clang::tidy::automotive

//===--- GenericCompatibleTypesCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericCompatibleTypesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericCompatibleTypesCheck::registerMatchers(MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("genericSelection"), this);
}

void GenericCompatibleTypesCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  ASTContext &Ctx = *Result.Context;
  unsigned NumAssocs = GenericSelection->getNumAssocs();

  // Collect all non-default association types
  llvm::SmallVector<std::pair<QualType, SourceLocation>, 8> Types;

  for (unsigned I = 0; I < NumAssocs; ++I) {
    auto Assoc = GenericSelection->getAssociation(I);
    // Skip default association (has null TypeSourceInfo)
    if (const TypeSourceInfo *TSI = Assoc.getTypeSourceInfo()) {
      QualType Type = TSI->getType();
      SourceLocation Loc = TSI->getTypeLoc().getBeginLoc();
      Types.push_back({Type, Loc});
    }
  }

  // Check for compatible types
  for (unsigned I = 0; I < Types.size(); ++I) {
    for (unsigned J = I + 1; J < Types.size(); ++J) {
      QualType Type1 = Types[I].first;
      QualType Type2 = Types[J].first;

      // Check if types are compatible
      // In C, use ASTContext::typesAreCompatible
      if (Ctx.typesAreCompatible(Type1, Type2)) {
        diag(Types[J].second,
             "generic association type '%0' is compatible with a previous "
             "association type '%1'")
            << Type2.getAsString() << Type1.getAsString();
        diag(Types[I].second, "previous association type here",
             DiagnosticIDs::Note);
      }
    }
  }
}

} // namespace clang::tidy::automotive

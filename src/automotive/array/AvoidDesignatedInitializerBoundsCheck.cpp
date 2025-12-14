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

void AvoidDesignatedInitializerBoundsCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *DIE = Result.Nodes.getNodeAs<DesignatedInitExpr>("designated");
  if (!DIE)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(DIE->getBeginLoc()))
    return;

  // Check each designator in the designation
  for (const auto &D : DIE->designators()) {
    // Check array designators for bounds
    if (D.isArrayDesignator()) {
      // Get the index expression
      const Expr *IndexExpr = DIE->getArrayIndex(D);
      if (!IndexExpr)
        continue;

      // Try to evaluate the index as a constant
      if (auto IndexValue =
              IndexExpr->getIntegerConstantExpr(*Result.Context)) {
        int64_t Index = IndexValue->getSExtValue();

        // Check if index is negative
        if (Index < 0) {
          diag(D.getLBracketLoc(),
               "designated initializer has negative array index %0")
              << static_cast<int>(Index);
          continue;
        }

        // Try to get the array bounds from the parent type
        // This requires walking up to find the InitListExpr and its type
      }
    }

    // Check array range designators (GNU extension)
    if (D.isArrayRangeDesignator()) {
      const Expr *StartExpr = DIE->getArrayRangeStart(D);
      const Expr *EndExpr = DIE->getArrayRangeEnd(D);

      if (StartExpr && EndExpr) {
        auto StartValue = StartExpr->getIntegerConstantExpr(*Result.Context);
        auto EndValue = EndExpr->getIntegerConstantExpr(*Result.Context);

        if (StartValue && EndValue) {
          int64_t Start = StartValue->getSExtValue();
          int64_t End = EndValue->getSExtValue();

          if (Start < 0 || End < 0) {
            diag(D.getLBracketLoc(),
                 "designated initializer array range has negative index");
          } else if (Start > End) {
            diag(D.getLBracketLoc(),
                 "designated initializer array range has start index %0 "
                 "greater than end index %1")
                << static_cast<int>(Start) << static_cast<int>(End);
          }
        }
      }
    }
  }
}

} // namespace clang::tidy::automotive

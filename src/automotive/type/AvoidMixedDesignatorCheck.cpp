//===--- AvoidMixedDesignatorCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidMixedDesignatorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidMixedDesignatorCheck::registerMatchers(MatchFinder *Finder) {
  // Match initializer lists
  Finder->addMatcher(initListExpr().bind("init"), this);
}

void AvoidMixedDesignatorCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Init = Result.Nodes.getNodeAs<InitListExpr>("init");
  if (!Init)
    return;

  // Get the syntactic form if available (for designated initializers)
  const InitListExpr *SyntacticInit =
      Init->isSyntacticForm() ? Init : Init->getSyntacticForm();

  if (!SyntacticInit)
    return;

  bool HasChainedDesignator = false;
  bool HasNonDesignated = false;
  SourceLocation FirstNonDesignatedLoc;

  for (unsigned I = 0; I < SyntacticInit->getNumInits(); ++I) {
    const Expr *InitExpr = SyntacticInit->getInit(I);
    if (!InitExpr)
      continue;

    // Check if this is a designated initializer
    if (const auto *DIE = dyn_cast<DesignatedInitExpr>(InitExpr)) {
      // Check if it has chained designators (more than one designator)
      // e.g., .a.b = value has 2 designators
      if (DIE->size() > 1) {
        HasChainedDesignator = true;
      }
    } else {
      // Non-designated initializer
      HasNonDesignated = true;
      if (!FirstNonDesignatedLoc.isValid()) {
        FirstNonDesignatedLoc = InitExpr->getBeginLoc();
      }
    }
  }

  // Warn if we have both chained designators and non-designated initializers
  if (HasChainedDesignator && HasNonDesignated) {
    diag(FirstNonDesignatedLoc,
         "initializer list with chained designators contains non-designated "
         "initializer; all initializers should use designators");
  }
}

} // namespace clang::tidy::automotive

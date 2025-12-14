//===--- AvoidPointerArithmeticBoundsCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPointerArithmeticBoundsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPointerArithmeticBoundsCheck::registerMatchers(MatchFinder *Finder) {
  // Rule 18.3: Relational operators on pointers
  Finder->addMatcher(binaryOperator(hasAnyOperatorName("<", ">", "<=", ">="),
                                    hasLHS(hasType(pointerType())),
                                    hasRHS(hasType(pointerType())))
                         .bind("relop"),
                     this);

  // Rule 18.2: Pointer subtraction
  Finder->addMatcher(binaryOperator(hasOperatorName("-"),
                                    hasLHS(hasType(pointerType())),
                                    hasRHS(hasType(pointerType())))
                         .bind("ptrsub"),
                     this);

  // Rule 18.5: Declaration with more than 2 levels of pointer
  Finder->addMatcher(varDecl().bind("vardecl"), this);
}

void AvoidPointerArithmeticBoundsCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Rule 18.3: Relational operators on pointers
  if (const auto *RelOp = Result.Nodes.getNodeAs<BinaryOperator>("relop")) {
    if (Result.SourceManager->isInSystemHeader(RelOp->getOperatorLoc()))
      return;

    diag(RelOp->getOperatorLoc(),
         "relational operator applied to pointers; ensure they address "
         "elements of the same array");
    return;
  }

  // Rule 18.2: Pointer subtraction
  if (const auto *PtrSub = Result.Nodes.getNodeAs<BinaryOperator>("ptrsub")) {
    if (Result.SourceManager->isInSystemHeader(PtrSub->getOperatorLoc()))
      return;

    diag(PtrSub->getOperatorLoc(),
         "pointer subtraction; ensure both pointers address elements of "
         "the same array object");
    return;
  }

  // Rule 18.5: More than 2 levels of pointer nesting
  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("vardecl")) {
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;

    unsigned NestingLevel = getPointerNestingLevel(VD->getType());
    if (NestingLevel > 2) {
      diag(VD->getLocation(),
           "declaration has %0 levels of pointer nesting; maximum allowed is 2")
          << NestingLevel;
    }
  }
}

unsigned
AvoidPointerArithmeticBoundsCheck::getPointerNestingLevel(QualType Type) const {
  unsigned Level = 0;
  while (Type->isPointerType()) {
    Level++;
    Type = Type->getPointeeType();
  }
  return Level;
}

} // namespace clang::tidy::automotive

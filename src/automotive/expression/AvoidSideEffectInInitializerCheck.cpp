//===--- AvoidSideEffectInInitializerCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSideEffectInInitializerCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class SideEffectCounter : public RecursiveASTVisitor<SideEffectCounter> {
public:
  unsigned getCount() const { return Count; }
  bool hasSideEffect() const { return Count > 0; }

  bool VisitUnaryOperator(UnaryOperator *UO) {
    if (UO->isIncrementDecrementOp())
      Count++;
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (BO->isAssignmentOp())
      Count++;
    return true;
  }

  bool VisitCallExpr(CallExpr *) {
    Count++;
    return true;
  }

private:
  unsigned Count = 0;
};

} // namespace

void AvoidSideEffectInInitializerCheck::registerMatchers(MatchFinder *Finder) {
  // Match initializer lists only - Rule 13.1 is specifically about initializer lists
  // Note: Rules 13.2/13.3 (unsequenced side effects) would need a separate check
  Finder->addMatcher(initListExpr().bind("initlist"), this);
}

void AvoidSideEffectInInitializerCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Rule 13.1: Side effects in initializer lists
  const auto *InitList = Result.Nodes.getNodeAs<InitListExpr>("initlist");
  if (!InitList)
    return;

  if (Result.SourceManager->isInSystemHeader(InitList->getBeginLoc()))
    return;

  for (const Expr *Init : InitList->inits()) {
    if (hasSideEffect(Init)) {
      diag(Init->getBeginLoc(),
           "initializer list element contains side effects which may not "
           "be evaluated in the expected order");
      return;
    }
  }
}

bool AvoidSideEffectInInitializerCheck::hasSideEffect(const Expr *E) const {
  SideEffectCounter Counter;
  // NOSONAR(S859): const_cast required by RecursiveASTVisitor API
  Counter.TraverseStmt(const_cast<Expr *>(E));
  return Counter.hasSideEffect();
}

} // namespace clang::tidy::automotive

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
  // Match initializer lists
  Finder->addMatcher(initListExpr().bind("initlist"), this);

  // Match expressions with multiple side effects (unsequenced)
  Finder->addMatcher(
      binaryOperator(unless(isAssignmentOperator())).bind("binexpr"), this);
}

void AvoidSideEffectInInitializerCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Rule 13.1: Side effects in initializer lists
  if (const auto *InitList = Result.Nodes.getNodeAs<InitListExpr>("initlist")) {
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
    return;
  }

  // Rules 13.2, 13.3: Multiple unsequenced side effects
  if (const auto *BinExpr = Result.Nodes.getNodeAs<BinaryOperator>("binexpr")) {
    if (Result.SourceManager->isInSystemHeader(BinExpr->getOperatorLoc()))
      return;

    unsigned LHSCount = countSideEffects(BinExpr->getLHS());
    unsigned RHSCount = countSideEffects(BinExpr->getRHS());

    if (LHSCount > 0 && RHSCount > 0) {
      diag(BinExpr->getOperatorLoc(),
           "expression contains multiple side effects with unspecified "
           "evaluation order");
    } else if (LHSCount > 1 || RHSCount > 1) {
      diag(BinExpr->getOperatorLoc(),
           "sub-expression contains multiple side effects");
    }
  }
}

bool AvoidSideEffectInInitializerCheck::hasSideEffect(const Expr *E) const {
  SideEffectCounter Counter;
  // NOSONAR(S859): const_cast required by RecursiveASTVisitor API
  Counter.TraverseStmt(const_cast<Expr *>(E));
  return Counter.hasSideEffect();
}

unsigned
AvoidSideEffectInInitializerCheck::countSideEffects(const Expr *E) const {
  SideEffectCounter Counter;
  // NOSONAR(S859): const_cast required by RecursiveASTVisitor API
  Counter.TraverseStmt(const_cast<Expr *>(E));
  return Counter.getCount();
}

} // namespace clang::tidy::automotive

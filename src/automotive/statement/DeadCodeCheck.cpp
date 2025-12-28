//===--- DeadCodeCheck.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DeadCodeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void DeadCodeCheck::registerMatchers(MatchFinder *Finder) {
  // Match expression statements that might have no effect
  Finder->addMatcher(
      expr(unless(isExpansionInSystemHeader()),
           hasParent(compoundStmt()),
           unless(hasParent(binaryOperator())),
           unless(hasParent(unaryOperator())),
           unless(hasParent(callExpr())),
           unless(hasParent(returnStmt())),
           unless(hasParent(ifStmt())),
           unless(hasParent(whileStmt())),
           unless(hasParent(forStmt())),
           unless(hasParent(doStmt())),
           unless(hasParent(switchStmt())),
           unless(hasParent(caseStmt())),
           unless(hasParent(defaultStmt())),
           unless(hasParent(varDecl())),
           unless(hasParent(arraySubscriptExpr())),
           unless(hasParent(memberExpr())),
           unless(hasParent(parenExpr())),
           unless(hasParent(cStyleCastExpr())),
           unless(hasParent(implicitCastExpr())),
           unless(hasParent(conditionalOperator())))
          .bind("deadExpr"),
      this);
}

bool DeadCodeCheck::hasSideEffects(const Expr *E) const {
  if (!E)
    return false;

  // Strip implicit casts and parentheses
  E = E->IgnoreParenImpCasts();

  // Call expressions have side effects
  if (isa<CallExpr>(E))
    return true;

  // Assignments have side effects
  if (const auto *BinOp = dyn_cast<BinaryOperator>(E)) {
    if (BinOp->isAssignmentOp())
      return true;
    // For non-assignment operators, check if operands have side effects
    return hasSideEffects(BinOp->getLHS()) || hasSideEffects(BinOp->getRHS());
  }

  // Increment/decrement have side effects
  if (const auto *UnOp = dyn_cast<UnaryOperator>(E)) {
    if (UnOp->isIncrementDecrementOp())
      return true;
    return hasSideEffects(UnOp->getSubExpr());
  }

  // Compound assignment has side effects
  if (isa<CompoundAssignOperator>(E))
    return true;

  // Comma operator - check right side
  if (const auto *Comma = dyn_cast<BinaryOperator>(E)) {
    if (Comma->getOpcode() == BO_Comma)
      return hasSideEffects(Comma->getRHS());
  }

  // Conditional operator - check both branches
  if (const auto *Cond = dyn_cast<ConditionalOperator>(E)) {
    return hasSideEffects(Cond->getTrueExpr()) ||
           hasSideEffects(Cond->getFalseExpr());
  }

  // Array subscript, member access - propagate check
  if (const auto *ArraySub = dyn_cast<ArraySubscriptExpr>(E))
    return hasSideEffects(ArraySub->getBase()) ||
           hasSideEffects(ArraySub->getIdx());

  if (const auto *Member = dyn_cast<MemberExpr>(E))
    return hasSideEffects(Member->getBase());

  // New/delete have side effects
  if (isa<CXXNewExpr>(E) || isa<CXXDeleteExpr>(E))
    return true;

  // Throw has side effects
  if (isa<CXXThrowExpr>(E))
    return true;

  // Literals, variable references, etc. have no side effects
  return false;
}

void DeadCodeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *E = Result.Nodes.getNodeAs<Expr>("deadExpr");
  if (!E)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(E->getBeginLoc()))
    return;

  // Skip expressions that are part of larger expressions
  // Only care about standalone expression statements
  const auto *Parent = Result.Context->getParents(*E).begin();
  if (Parent && !Parent->get<CompoundStmt>())
    return;

  // Check if the expression has side effects
  if (!hasSideEffects(E)) {
    // Get a description of the expression type
    StringRef ExprDesc = "expression";
    if (isa<DeclRefExpr>(E))
      ExprDesc = "variable reference";
    else if (isa<IntegerLiteral>(E) || isa<FloatingLiteral>(E) ||
             isa<StringLiteral>(E))
      ExprDesc = "literal";
    else if (isa<BinaryOperator>(E))
      ExprDesc = "operation";
    else if (isa<UnaryOperator>(E))
      ExprDesc = "operation";

    diag(E->getBeginLoc(), "dead code: %0 has no effect") << ExprDesc;
  }
}

} // namespace clang::tidy::automotive

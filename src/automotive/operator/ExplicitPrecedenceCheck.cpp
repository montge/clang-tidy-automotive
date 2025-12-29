//===--- ExplicitPrecedenceCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExplicitPrecedenceCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ExplicitPrecedenceCheck::registerMatchers(MatchFinder *Finder) {
  // Match binary operators that have binary operator children
  Finder->addMatcher(
      binaryOperator(
          unless(isExpansionInSystemHeader()),
          anyOf(hasLHS(ignoringParenImpCasts(
                    binaryOperator().bind("leftChild"))),
                hasRHS(ignoringParenImpCasts(
                    binaryOperator().bind("rightChild")))))
          .bind("parent"),
      this);
}

bool ExplicitPrecedenceCheck::isConfusingPrecedence(
    BinaryOperatorKind Parent, BinaryOperatorKind Child) const {
  // Shift with arithmetic is confusing: a + b << c
  // User might expect (a + b) << c or a + (b << c)
  bool ParentIsShift = (Parent == BO_Shl || Parent == BO_Shr);
  bool ChildIsShift = (Child == BO_Shl || Child == BO_Shr);
  bool ParentIsAddSub = (Parent == BO_Add || Parent == BO_Sub);
  bool ChildIsAddSub = (Child == BO_Add || Child == BO_Sub);
  bool ParentIsMulDiv =
      (Parent == BO_Mul || Parent == BO_Div || Parent == BO_Rem);
  bool ChildIsMulDiv =
      (Child == BO_Mul || Child == BO_Div || Child == BO_Rem);

  if (ParentIsShift && (ChildIsAddSub || ChildIsMulDiv))
    return true;
  if (ChildIsShift && (ParentIsAddSub || ParentIsMulDiv))
    return true;

  // Bitwise with comparison is confusing: a & b == c
  // User might expect (a & b) == c or a & (b == c)
  bool ParentIsBitwise =
      (Parent == BO_And || Parent == BO_Or || Parent == BO_Xor);
  bool ChildIsBitwise =
      (Child == BO_And || Child == BO_Or || Child == BO_Xor);
  bool ParentIsComparison = (Parent == BO_EQ || Parent == BO_NE ||
                             Parent == BO_LT || Parent == BO_GT ||
                             Parent == BO_LE || Parent == BO_GE);
  bool ChildIsComparison = (Child == BO_EQ || Child == BO_NE ||
                            Child == BO_LT || Child == BO_GT ||
                            Child == BO_LE || Child == BO_GE);

  if (ParentIsBitwise && ChildIsComparison)
    return true;
  if (ChildIsBitwise && ParentIsComparison)
    return true;

  // Bitwise with logical is confusing: a & b && c
  bool ParentIsLogical = (Parent == BO_LAnd || Parent == BO_LOr);
  bool ChildIsLogical = (Child == BO_LAnd || Child == BO_LOr);

  if (ParentIsBitwise && ChildIsLogical)
    return true;
  if (ChildIsBitwise && ParentIsLogical)
    return true;

  // Different bitwise operators mixed: a & b | c
  if (ParentIsBitwise && ChildIsBitwise && Parent != Child)
    return true;

  return false;
}

void ExplicitPrecedenceCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Parent = Result.Nodes.getNodeAs<BinaryOperator>("parent");
  const auto *LeftChild = Result.Nodes.getNodeAs<BinaryOperator>("leftChild");
  const auto *RightChild =
      Result.Nodes.getNodeAs<BinaryOperator>("rightChild");

  if (!Parent)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Parent->getOperatorLoc()))
    return;

  BinaryOperatorKind ParentOp = Parent->getOpcode();

  // Check left child
  if (LeftChild) {
    // If the left child is wrapped in parentheses, it's fine
    const Expr *LHS = Parent->getLHS()->IgnoreImpCasts();
    if (!isa<ParenExpr>(LHS)) {
      if (isConfusingPrecedence(ParentOp, LeftChild->getOpcode())) {
        diag(Parent->getOperatorLoc(),
             "operator precedence may be confusing; use parentheses to clarify "
             "intent")
            << FixItHint::CreateInsertion(LeftChild->getBeginLoc(), "(")
            << FixItHint::CreateInsertion(
                   LeftChild->getEndLoc().getLocWithOffset(1), ")");
        return;
      }
    }
  }

  // Check right child
  if (RightChild) {
    // If the right child is wrapped in parentheses, it's fine
    const Expr *RHS = Parent->getRHS()->IgnoreImpCasts();
    if (!isa<ParenExpr>(RHS)) {
      if (isConfusingPrecedence(ParentOp, RightChild->getOpcode())) {
        diag(Parent->getOperatorLoc(),
             "operator precedence may be confusing; use parentheses to clarify "
             "intent")
            << FixItHint::CreateInsertion(RightChild->getBeginLoc(), "(")
            << FixItHint::CreateInsertion(
                   RightChild->getEndLoc().getLocWithOffset(1), ")");
        return;
      }
    }
  }
}

} // namespace clang::tidy::automotive

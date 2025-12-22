//===--- AvoidAssignmentResultCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidAssignmentResultCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidAssignmentResultCheck::registerMatchers(MatchFinder *Finder) {
  // Match assignment operators that are used as sub-expressions
  // We need to detect when the result of an assignment is used,
  // except for chained assignments (a = b = c)

  // Assignment in if condition
  Finder->addMatcher(
      ifStmt(hasCondition(expr(hasDescendant(
                 binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("if"),
      this);

  // Assignment in while condition
  Finder->addMatcher(
      whileStmt(
          hasCondition(expr(hasDescendant(
              binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("while"),
      this);

  // Assignment in do-while condition
  Finder->addMatcher(
      doStmt(hasCondition(expr(hasDescendant(
                 binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("do"),
      this);

  // Assignment in for condition (not init or increment)
  Finder->addMatcher(
      forStmt(hasCondition(expr(hasDescendant(
                  binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("forCond"),
      this);

  // Assignment inside another binary operator (except simple assignment or
  // comma) This catches cases like: y = (x = 5) + 1 and z += (x = y)
  Finder->addMatcher(
      binaryOperator(
          unless(hasOperatorName("=")), unless(hasOperatorName(",")),
          hasDescendant(
              binaryOperator(isAssignmentOperator()).bind("assignment")))
          .bind("binop"),
      this);

  // Assignment in return statement
  Finder->addMatcher(
      returnStmt(
          hasReturnValue(expr(hasDescendant(
              binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("return"),
      this);

  // Assignment in ternary conditional condition
  Finder->addMatcher(
      conditionalOperator(
          hasCondition(expr(hasDescendant(
              binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("ternary"),
      this);

  // Assignment in function call argument
  Finder->addMatcher(
      callExpr(hasAnyArgument(expr(hasDescendant(
                   binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("call"),
      this);

  // Assignment in array subscript
  Finder->addMatcher(
      arraySubscriptExpr(
          hasIndex(expr(hasDescendant(
              binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("subscript"),
      this);

  // Assignment in unary operator
  Finder->addMatcher(
      unaryOperator(
          hasUnaryOperand(expr(hasDescendant(
              binaryOperator(isAssignmentOperator()).bind("assignment")))))
          .bind("unary"),
      this);
}

void AvoidAssignmentResultCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Assignment = Result.Nodes.getNodeAs<BinaryOperator>("assignment");
  if (!Assignment)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Assignment->getExprLoc()))
    return;

  // Check if this is part of a chained assignment (a = b = c)
  // In chained assignments, the inner assignment is the RHS of the outer one
  ASTContext &Context = *Result.Context;
  auto Parents = Context.getParents(*Assignment);

  if (!Parents.empty()) {
    const auto *ParentExpr = Parents[0].get<Expr>();
    if (ParentExpr) {
      // Check if parent is another assignment operator
      if (const auto *ParentBinOp = dyn_cast<BinaryOperator>(ParentExpr)) {
        if (ParentBinOp->isAssignmentOp()) {
          // Check if this assignment is the RHS of the parent assignment
          if (ParentBinOp->getRHS()->IgnoreParenImpCasts() ==
              Assignment->IgnoreParenImpCasts()) {
            // This is a chained assignment, which is allowed
            return;
          }
        }
      }
    }
  }

  diag(Assignment->getOperatorLoc(),
       "the result of an assignment shall not be used");
}

} // namespace clang::tidy::automotive::cpp23

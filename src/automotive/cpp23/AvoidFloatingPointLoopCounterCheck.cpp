//===--- AvoidFloatingPointLoopCounterCheck.cpp - clang-tidy --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidFloatingPointLoopCounterCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidFloatingPointLoopCounterCheck::registerMatchers(MatchFinder *Finder) {
  // Match for loops with floating-point variable declarations in init
  Finder->addMatcher(
      forStmt(hasLoopInit(declStmt(containsDeclaration(
                  0, varDecl(hasType(realFloatingPointType())).bind("var")))))
          .bind("for"),
      this);

  // Match for loops with floating-point assignment in init
  Finder->addMatcher(
      forStmt(
          hasLoopInit(binaryOperator(
              isAssignmentOperator(),
              hasLHS(
                  declRefExpr(hasType(realFloatingPointType())).bind("ref")))))
          .bind("forAssign"),
      this);

  // Match for loops with floating-point increment/decrement in increment expr
  Finder->addMatcher(
      forStmt(hasIncrement(unaryOperator(
                  anyOf(hasOperatorName("++"), hasOperatorName("--")),
                  hasUnaryOperand(
                      expr(hasType(realFloatingPointType())).bind("incExpr")))))
          .bind("forUnary"),
      this);

  // Match for loops with compound assignment on floating-point in increment
  Finder->addMatcher(
      forStmt(
          hasIncrement(binaryOperator(
              isAssignmentOperator(),
              hasLHS(expr(hasType(realFloatingPointType())).bind("incLhs")))))
          .bind("forCompound"),
      this);
}

void AvoidFloatingPointLoopCounterCheck::check(
    const MatchFinder::MatchResult &Result) {
  SourceLocation Loc;
  std::string TypeName;

  if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
    if (Result.SourceManager->isInSystemHeader(Var->getLocation()))
      return;
    Loc = Var->getLocation();
    TypeName = Var->getType().getAsString();
  } else if (const auto *Ref = Result.Nodes.getNodeAs<DeclRefExpr>("ref")) {
    if (Result.SourceManager->isInSystemHeader(Ref->getLocation()))
      return;
    Loc = Ref->getLocation();
    TypeName = Ref->getType().getAsString();
  } else if (const auto *Inc = Result.Nodes.getNodeAs<Expr>("incExpr")) {
    if (Result.SourceManager->isInSystemHeader(Inc->getExprLoc()))
      return;
    Loc = Inc->getExprLoc();
    TypeName = Inc->getType().getAsString();
  } else if (const auto *IncLhs = Result.Nodes.getNodeAs<Expr>("incLhs")) {
    if (Result.SourceManager->isInSystemHeader(IncLhs->getExprLoc()))
      return;
    Loc = IncLhs->getExprLoc();
    TypeName = IncLhs->getType().getAsString();
  } else {
    return;
  }

  diag(Loc,
       "floating-point type '%0' used as loop counter; use an integer type "
       "instead to avoid precision issues")
      << TypeName;
}

} // namespace clang::tidy::automotive

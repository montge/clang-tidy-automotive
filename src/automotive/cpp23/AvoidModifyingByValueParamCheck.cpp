//===--- AvoidModifyingByValueParamCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidModifyingByValueParamCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Check if a ParmVarDecl is passed by value (not pointer, not reference)
AST_MATCHER(ParmVarDecl, isByValue) {
  QualType Type = Node.getType();
  return !Type->isReferenceType() && !Type->isPointerType();
}

} // namespace

void AvoidModifyingByValueParamCheck::registerMatchers(MatchFinder *Finder) {
  // Match assignments to by-value parameters
  Finder->addMatcher(
      binaryOperator(
          isAssignmentOperator(),
          hasLHS(declRefExpr(to(parmVarDecl(isByValue()).bind("param")))))
          .bind("assignment"),
      this);

  // Match unary increment/decrement operators on by-value parameters
  Finder->addMatcher(
      unaryOperator(anyOf(hasOperatorName("++"), hasOperatorName("--")),
                    hasUnaryOperand(declRefExpr(
                        to(parmVarDecl(isByValue()).bind("param")))))
          .bind("unary"),
      this);
}

void AvoidModifyingByValueParamCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Param = Result.Nodes.getNodeAs<ParmVarDecl>("param");
  if (!Param)
    return;

  SourceLocation Loc;
  StringRef OpType;

  if (const auto *Assignment =
          Result.Nodes.getNodeAs<BinaryOperator>("assignment")) {
    if (Result.SourceManager->isInSystemHeader(Assignment->getOperatorLoc()))
      return;
    Loc = Assignment->getOperatorLoc();
    OpType = "assignment";
  } else if (const auto *Unary =
                 Result.Nodes.getNodeAs<UnaryOperator>("unary")) {
    if (Result.SourceManager->isInSystemHeader(Unary->getOperatorLoc()))
      return;
    Loc = Unary->getOperatorLoc();
    OpType = Unary->isIncrementOp() ? "increment" : "decrement";
  }

  if (Loc.isValid()) {
    diag(Loc, "%0 of parameter '%1' passed by value; modifications do not "
              "affect the caller")
        << OpType << Param->getName();
  }
}

} // namespace clang::tidy::automotive

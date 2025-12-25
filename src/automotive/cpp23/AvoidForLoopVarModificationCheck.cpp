//===--- AvoidForLoopVarModificationCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidForLoopVarModificationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Matcher to get the loop variable from the init statement of a for loop
AST_MATCHER_P(ForStmt, hasLoopVariable, DeclarationMatcher, InnerMatcher) {
  const Stmt *Init = Node.getInit();
  if (!Init)
    return false;

  if (const auto *DS = dyn_cast<DeclStmt>(Init)) {
    if (DS->isSingleDecl()) {
      if (const auto *VD = dyn_cast<VarDecl>(DS->getSingleDecl())) {
        return InnerMatcher.matches(*VD, Finder, Builder);
      }
    }
  }
  return false;
}

} // namespace

void AvoidForLoopVarModificationCheck::registerMatchers(MatchFinder *Finder) {
  // Match for statements with a declared loop variable
  auto LoopWithVar =
      forStmt(hasLoopVariable(varDecl().bind("loopVar"))).bind("forStmt");

  // Match assignments to the loop variable in the body
  Finder->addMatcher(
      forStmt(hasLoopVariable(varDecl().bind("loopVar")),
              hasBody(hasDescendant(
                  binaryOperator(isAssignmentOperator(),
                                 hasLHS(declRefExpr(
                                     to(varDecl(equalsBoundNode("loopVar"))))))
                      .bind("assignment")))),
      this);

  // Match unary increment/decrement operators on the loop variable in the body
  Finder->addMatcher(
      forStmt(
          hasLoopVariable(varDecl().bind("loopVar")),
          hasBody(hasDescendant(
              unaryOperator(anyOf(hasOperatorName("++"), hasOperatorName("--")),
                            hasUnaryOperand(declRefExpr(
                                to(varDecl(equalsBoundNode("loopVar"))))))
                  .bind("unary")))),
      this);
}

void AvoidForLoopVarModificationCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *LoopVar = Result.Nodes.getNodeAs<VarDecl>("loopVar");
  if (!LoopVar)
    return;

  SourceLocation Loc;
  std::string Message;

  if (const auto *Assignment =
          Result.Nodes.getNodeAs<BinaryOperator>("assignment")) {
    if (Result.SourceManager->isInSystemHeader(Assignment->getOperatorLoc()))
      return;
    Loc = Assignment->getOperatorLoc();
    Message = "loop variable '" + LoopVar->getName().str() +
              "' should not be modified in the loop body";
  } else if (const auto *Unary =
                 Result.Nodes.getNodeAs<UnaryOperator>("unary")) {
    if (Result.SourceManager->isInSystemHeader(Unary->getOperatorLoc()))
      return;
    Loc = Unary->getOperatorLoc();
    Message = "loop variable '" + LoopVar->getName().str() +
              "' should not be modified in the loop body";
  }

  if (Loc.isValid()) {
    diag(Loc, Message);
  }
}

} // namespace clang::tidy::automotive

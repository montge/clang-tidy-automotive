//===--- BooleanControlExpressionCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "BooleanControlExpressionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void BooleanControlExpressionCheck::registerMatchers(MatchFinder *Finder) {
  // Match if statements
  Finder->addMatcher(ifStmt().bind("if_stmt"), this);

  // Match while loops
  Finder->addMatcher(whileStmt().bind("while_stmt"), this);

  // Match do-while loops
  Finder->addMatcher(doStmt().bind("do_stmt"), this);

  // Match for loops
  Finder->addMatcher(forStmt().bind("for_stmt"), this);

  // Match conditional operator (ternary)
  Finder->addMatcher(conditionalOperator().bind("ternary"), this);
}

bool BooleanControlExpressionCheck::isEssentiallyBoolean(
    const Expr *E, ASTContext *Context) const {

  if (!E)
    return false;

  E = E->IgnoreParenImpCasts();

  QualType Type = E->getType();

  // Direct Boolean type is OK
  if (Type->isBooleanType())
    return true;

  // Comparison operators return Boolean
  if (const auto *BinOp = dyn_cast<BinaryOperator>(E)) {
    if (BinOp->isComparisonOp() || BinOp->isEqualityOp())
      return true;

    // Logical operators are Boolean
    if (BinOp->isLogicalOp())
      return true;
  }

  // Unary logical not is Boolean
  if (const auto *UnOp = dyn_cast<UnaryOperator>(E)) {
    if (UnOp->getOpcode() == UO_LNot)
      return true;
  }

  // Call expressions that return bool
  if (Type->isBooleanType())
    return true;

  return false;
}

void BooleanControlExpressionCheck::check(
    const MatchFinder::MatchResult &Result) {

  const Expr *CondExpr = nullptr;
  StringRef StmtType;

  if (const auto *If = Result.Nodes.getNodeAs<IfStmt>("if_stmt")) {
    CondExpr = If->getCond();
    StmtType = "if";
  } else if (const auto *While = Result.Nodes.getNodeAs<WhileStmt>("while_stmt")) {
    CondExpr = While->getCond();
    StmtType = "while";
  } else if (const auto *Do = Result.Nodes.getNodeAs<DoStmt>("do_stmt")) {
    CondExpr = Do->getCond();
    StmtType = "do-while";
  } else if (const auto *For = Result.Nodes.getNodeAs<ForStmt>("for_stmt")) {
    CondExpr = For->getCond();
    StmtType = "for";
  } else if (const auto *Ternary =
                 Result.Nodes.getNodeAs<ConditionalOperator>("ternary")) {
    CondExpr = Ternary->getCond();
    StmtType = "ternary operator";
  }

  if (!CondExpr)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(CondExpr->getBeginLoc()))
    return;

  // Strip implicit casts and parentheses for analysis
  const Expr *StrippedExpr = CondExpr->IgnoreParenImpCasts();

  // Check if the condition is essentially Boolean
  if (!isEssentiallyBoolean(StrippedExpr, Result.Context)) {
    QualType ExprType = StrippedExpr->getType();

    diag(CondExpr->getBeginLoc(),
         "control expression in %0 statement shall be of essentially Boolean "
         "type; found %1 (MISRA C++:2023 Rule 16.5)")
        << StmtType << ExprType;
  }
}

} // namespace clang::tidy::automotive::cpp23

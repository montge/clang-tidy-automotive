//===--- AvoidCompositeNarrowingCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCompositeNarrowingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidCompositeNarrowingCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with initializers that are binary operations
  Finder->addMatcher(varDecl(hasInitializer(ignoringParenImpCasts(
                                 binaryOperator().bind("binop"))))
                         .bind("var"),
                     this);

  // Match assignments where the RHS is a binary operation
  Finder->addMatcher(
      binaryOperator(hasOperatorName("="), hasRHS(ignoringParenImpCasts(
                                               binaryOperator().bind("binop"))))
          .bind("assign"),
      this);
}

void AvoidCompositeNarrowingCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop");
  if (!BinOp)
    return;

  // Skip if in system header
  if (Result.Context->getSourceManager().isInSystemHeader(BinOp->getBeginLoc()))
    return;

  // Check if this is a composite expression (arithmetic operation)
  if (!isCompositeExpression(BinOp))
    return;

  QualType ExprType = BinOp->getType().getCanonicalType();
  QualType DestType;

  // Determine the destination type
  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    DestType = VD->getType().getCanonicalType();
  } else if (const auto *AssignOp =
                 Result.Nodes.getNodeAs<BinaryOperator>("assign")) {
    DestType = AssignOp->getLHS()->getType().getCanonicalType();
  } else {
    return;
  }

  // Check if this is a narrowing assignment
  if (isNarrowingAssignment(ExprType, DestType, *Result.Context)) {
    diag(BinOp->getBeginLoc(),
         "composite expression result of type %0 shall not be assigned to "
         "narrower type %1; use explicit cast if narrowing is intentional")
        << ExprType << DestType;
  }
}

bool AvoidCompositeNarrowingCheck::isCompositeExpression(const Expr *E) const {
  const auto *BinOp = dyn_cast<BinaryOperator>(E);
  if (!BinOp)
    return false;

  // Check if this is an arithmetic or bitwise operation
  // These are the operations that produce composite expressions
  return BinOp->isAdditiveOp() ||       // +, -
         BinOp->isMultiplicativeOp() || // *, /, %
         BinOp->isShiftOp() ||          // <<, >>
         BinOp->isBitwiseOp();          // &, |, ^
}

bool AvoidCompositeNarrowingCheck::isNarrowingAssignment(
    QualType ExprType, QualType DestType, const ASTContext &Ctx) const {
  // Get canonical types without qualifiers
  ExprType = ExprType.getCanonicalType().getUnqualifiedType();
  DestType = DestType.getCanonicalType().getUnqualifiedType();

  // Same type is not narrowing
  if (ExprType == DestType)
    return false;

  // Both must be arithmetic types
  if (!ExprType->isArithmeticType() || !DestType->isArithmeticType())
    return false;

  // Floating-point to integer is always narrowing
  if (ExprType->isFloatingType() && DestType->isIntegerType())
    return true;

  // Larger floating-point to smaller floating-point
  if (ExprType->isFloatingType() && DestType->isFloatingType()) {
    unsigned ExprSize = Ctx.getTypeSize(ExprType);
    unsigned DestSize = Ctx.getTypeSize(DestType);
    return ExprSize > DestSize;
  }

  // Integer to integer: check size
  if (ExprType->isIntegerType() && DestType->isIntegerType()) {
    unsigned ExprSize = Ctx.getTypeSize(ExprType);
    unsigned DestSize = Ctx.getTypeSize(DestType);

    // Larger to smaller is narrowing
    if (ExprSize > DestSize)
      return true;

    // Same size but different signedness can be narrowing
    if (ExprSize == DestSize) {
      bool ExprSigned = ExprType->isSignedIntegerType();
      bool DestSigned = DestType->isSignedIntegerType();

      // Signed to unsigned of same size can lose the sign
      // Unsigned to signed of same size can lose magnitude
      if (ExprSigned != DestSigned)
        return true;
    }
  }

  return false;
}

} // namespace clang::tidy::automotive::cpp23

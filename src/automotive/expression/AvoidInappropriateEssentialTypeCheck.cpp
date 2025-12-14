//===--- AvoidInappropriateEssentialTypeCheck.cpp - clang-tidy -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidInappropriateEssentialTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidInappropriateEssentialTypeCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match binary operators: arithmetic, bitwise, and relational
  Finder->addMatcher(
      binaryOperator(anyOf(hasAnyOperatorName("+", "-", "*", "/", "%"),
                           hasAnyOperatorName("&", "|", "^", "<<", ">>"),
                           hasAnyOperatorName("+=", "-=", "*=", "/=", "%=", "&=",
                                              "|=", "^=", "<<=", ">>=")))
          .bind("binop"),
      this);

  // Match unary operators that may have essential type issues
  Finder->addMatcher(
      unaryOperator(anyOf(hasOperatorName("~"), hasOperatorName("+"),
                          hasOperatorName("-")))
          .bind("unop"),
      this);
}

void AvoidInappropriateEssentialTypeCheck::checkArithmeticOperand(
    const Expr *Operand, EssentialType ET) {
  if (!isAppropriateForArithmetic(ET)) {
    diag(Operand->getBeginLoc(),
         "operand of essential type '%0' is inappropriate for arithmetic "
         "operation")
        << getEssentialTypeName(ET);
  }
}

void AvoidInappropriateEssentialTypeCheck::checkBitwiseOperand(
    const Expr *Operand, EssentialType ET) {
  if (!isAppropriateForBitwise(ET)) {
    diag(Operand->getBeginLoc(),
         "operand of essential type '%0' is inappropriate for bitwise "
         "operation")
        << getEssentialTypeName(ET);
  }
}

void AvoidInappropriateEssentialTypeCheck::handleBinaryOperator(
    const BinaryOperator *BinOp, const SourceManager &SM) {
  if (SM.isInSystemHeader(BinOp->getOperatorLoc()))
    return;

  const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
  const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

  EssentialType LHSET = getEssentialType(LHS->getType());
  EssentialType RHSET = getEssentialType(RHS->getType());

  bool IsArithmetic = BinOp->isAdditiveOp() || BinOp->isMultiplicativeOp();
  bool IsBitwise = BinOp->isBitwiseOp() || BinOp->isShiftOp();

  if (IsArithmetic) {
    checkArithmeticOperand(LHS, LHSET);
    checkArithmeticOperand(RHS, RHSET);
  }

  if (IsBitwise) {
    checkBitwiseOperand(LHS, LHSET);
    checkBitwiseOperand(RHS, RHSET);
  }
}

void AvoidInappropriateEssentialTypeCheck::handleUnaryOperator(
    const UnaryOperator *UnOp, const SourceManager &SM) {
  if (SM.isInSystemHeader(UnOp->getOperatorLoc()))
    return;

  const Expr *SubExpr = UnOp->getSubExpr()->IgnoreParenImpCasts();
  EssentialType ET = getEssentialType(SubExpr->getType());

  if (UnOp->getOpcode() == UO_Not) {
    checkBitwiseOperand(SubExpr, ET);
    return;
  }

  if (UnOp->getOpcode() == UO_Plus || UnOp->getOpcode() == UO_Minus) {
    if (!isAppropriateForArithmetic(ET)) {
      diag(SubExpr->getBeginLoc(),
           "operand of essential type '%0' is inappropriate for unary '%1' "
           "operation")
          << getEssentialTypeName(ET)
          << (UnOp->getOpcode() == UO_Plus ? "+" : "-");
    }
  }
}

void AvoidInappropriateEssentialTypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop")) {
    handleBinaryOperator(BinOp, *Result.SourceManager);
    return;
  }

  if (const auto *UnOp = Result.Nodes.getNodeAs<UnaryOperator>("unop")) {
    handleUnaryOperator(UnOp, *Result.SourceManager);
  }
}

AvoidInappropriateEssentialTypeCheck::EssentialType
AvoidInappropriateEssentialTypeCheck::getEssentialType(QualType Type) const {
  Type = Type.getCanonicalType().getUnqualifiedType();

  if (Type->isBooleanType())
    return EssentialType::Boolean;

  if (Type->isAnyCharacterType())
    return EssentialType::Character;

  if (Type->isEnumeralType())
    return EssentialType::Enum;

  if (Type->isFloatingType())
    return EssentialType::FloatingPoint;

  if (Type->isIntegerType()) {
    if (Type->isUnsignedIntegerType())
      return EssentialType::UnsignedInt;
    if (Type->isSignedIntegerType())
      return EssentialType::SignedInt;
  }

  return EssentialType::Other;
}

bool AvoidInappropriateEssentialTypeCheck::isAppropriateForArithmetic(
    EssentialType ET) const {
  return ET != EssentialType::Boolean && ET != EssentialType::Character;
}

bool AvoidInappropriateEssentialTypeCheck::isAppropriateForBitwise(
    EssentialType ET) const {
  return ET != EssentialType::Boolean && ET != EssentialType::Character &&
         ET != EssentialType::FloatingPoint;
}

StringRef AvoidInappropriateEssentialTypeCheck::getEssentialTypeName(
    EssentialType ET) const {
  switch (ET) {
  case EssentialType::Boolean:
    return "Boolean";
  case EssentialType::Character:
    return "Character";
  case EssentialType::SignedInt:
    return "Signed";
  case EssentialType::UnsignedInt:
    return "Unsigned";
  case EssentialType::FloatingPoint:
    return "Floating";
  case EssentialType::Enum:
    return "Enum";
  case EssentialType::Other:
    return "Other";
  }
  return "Unknown";
}

} // namespace clang::tidy::automotive

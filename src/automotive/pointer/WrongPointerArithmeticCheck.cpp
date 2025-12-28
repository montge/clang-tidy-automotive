//===--- WrongPointerArithmeticCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "WrongPointerArithmeticCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void WrongPointerArithmeticCheck::registerMatchers(MatchFinder *Finder) {
  // Match pointer arithmetic operations: ++, --, +, -, +=, -=
  auto PointerType = hasType(pointerType());

  // Unary increment/decrement
  Finder->addMatcher(
      unaryOperator(hasAnyOperatorName("++", "--"),
                    hasUnaryOperand(PointerType))
          .bind("unary_op"),
      this);

  // Binary arithmetic: pointer + integer, pointer - integer
  Finder->addMatcher(
      binaryOperator(hasAnyOperatorName("+", "-", "+=", "-="),
                     hasEitherOperand(PointerType))
          .bind("binary_op"),
      this);

  // Array subscript (pointer[index])
  Finder->addMatcher(arraySubscriptExpr(hasBase(PointerType)).bind("subscript"),
                     this);
}

bool WrongPointerArithmeticCheck::isArrayElementPointer(const Expr *E,
                                                         ASTContext &Context) {
  if (!E)
    return false;

  E = E->IgnoreParenImpCasts();

  // Check if it's an array-to-pointer decay
  if (auto *ICE = dyn_cast<ImplicitCastExpr>(E)) {
    if (ICE->getCastKind() == CK_ArrayToPointerDecay) {
      return true;
    }
  }

  // Check if derived from array subscript
  if (isa<ArraySubscriptExpr>(E))
    return true;

  // Check if it's a pointer to an array element via unary operator &
  if (auto *UO = dyn_cast<UnaryOperator>(E)) {
    if (UO->getOpcode() == UO_AddrOf) {
      const Expr *SubExpr = UO->getSubExpr()->IgnoreParenImpCasts();
      if (isa<ArraySubscriptExpr>(SubExpr))
        return true;
    }
  }

  // Check if referencing a variable declared as array
  if (auto *DRE = dyn_cast<DeclRefExpr>(E)) {
    if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
      QualType VarType = VD->getType();
      if (VarType->isArrayType())
        return true;
    }
  }

  // Check for pointer arithmetic (already an array pointer)
  if (auto *BO = dyn_cast<BinaryOperator>(E)) {
    if (BO->isAdditiveOp()) {
      return isArrayElementPointer(BO->getLHS(), Context) ||
             isArrayElementPointer(BO->getRHS(), Context);
    }
  }

  return false;
}

void WrongPointerArithmeticCheck::check(
    const MatchFinder::MatchResult &Result) {
  const Expr *PointerExpr = nullptr;
  SourceLocation DiagLoc;

  if (const auto *UO = Result.Nodes.getNodeAs<UnaryOperator>("unary_op")) {
    if (Result.SourceManager->isInSystemHeader(UO->getOperatorLoc()))
      return;
    PointerExpr = UO->getSubExpr();
    DiagLoc = UO->getOperatorLoc();
  } else if (const auto *BO =
                 Result.Nodes.getNodeAs<BinaryOperator>("binary_op")) {
    if (Result.SourceManager->isInSystemHeader(BO->getOperatorLoc()))
      return;

    // Determine which operand is the pointer
    if (BO->getLHS()->getType()->isPointerType())
      PointerExpr = BO->getLHS();
    else if (BO->getRHS()->getType()->isPointerType())
      PointerExpr = BO->getRHS();
    else
      return; // Neither operand is a pointer

    DiagLoc = BO->getOperatorLoc();
  } else if (const auto *ASE =
                 Result.Nodes.getNodeAs<ArraySubscriptExpr>("subscript")) {
    if (Result.SourceManager->isInSystemHeader(ASE->getBeginLoc()))
      return;
    PointerExpr = ASE->getBase();
    DiagLoc = ASE->getBeginLoc();
  }

  if (!PointerExpr)
    return;

  // Check if pointer refers to array elements
  if (!isArrayElementPointer(PointerExpr, *Result.Context)) {
    diag(DiagLoc, "pointer arithmetic shall only be applied to pointers that "
                  "address array elements");
  }
}

} // namespace clang::tidy::automotive

//===--- AvoidPointerArithmeticC25Check.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPointerArithmeticC25Check.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPointerArithmeticC25Check::registerMatchers(MatchFinder *Finder) {
  // Match binary + and - operators with pointer operands
  // ptr + n, n + ptr, ptr - n, ptr1 - ptr2
  Finder->addMatcher(
      binaryOperator(
          anyOf(hasOperatorName("+"), hasOperatorName("-")),
          anyOf(hasLHS(hasType(pointerType())), hasRHS(hasType(pointerType()))))
          .bind("binaryOp"),
      this);

  // Match compound assignment operators += and -= on pointers
  // ptr += n, ptr -= n
  // Use binaryOperator since CompoundAssignOperator is a subclass
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("+="), hasOperatorName("-=")),
                     hasLHS(hasType(pointerType())))
          .bind("compoundOp"),
      this);
}

void AvoidPointerArithmeticC25Check::check(
    const MatchFinder::MatchResult &Result) {

  // Handle binary + and - operators
  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binaryOp")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(BinOp->getBeginLoc()))
      return;

    // Skip if part of array subscript expression (arr[i] is equivalent to *(arr
    // + i)) We check if this binary operator is the direct child of an array
    // subscript
    const auto Parents = Result.Context->getParents(*BinOp);
    for (const auto &Parent : Parents) {
      if (Parent.get<ArraySubscriptExpr>())
        return;
    }

    QualType LHSType = BinOp->getLHS()->getType();
    QualType RHSType = BinOp->getRHS()->getType();

    if (BinOp->getOpcode() == BO_Add) {
      // For addition, check if either operand is a pointer
      if (LHSType->isPointerType() && RHSType->isIntegerType()) {
        diag(BinOp->getOperatorLoc(),
             "pointer arithmetic using '+' operator on pointer type %0; "
             "use array subscript notation instead")
            << LHSType;
      } else if (LHSType->isIntegerType() && RHSType->isPointerType()) {
        diag(BinOp->getOperatorLoc(),
             "pointer arithmetic using '+' operator on pointer type %0; "
             "use array subscript notation instead")
            << RHSType;
      }
    } else if (BinOp->getOpcode() == BO_Sub) {
      // For subtraction, both operands being pointers or pointer - integer
      if (LHSType->isPointerType()) {
        if (RHSType->isPointerType()) {
          diag(BinOp->getOperatorLoc(), "pointer difference using '-' operator "
                                        "on pointer types %0 and %1")
              << LHSType << RHSType;
        } else if (RHSType->isIntegerType()) {
          diag(BinOp->getOperatorLoc(),
               "pointer arithmetic using '-' operator on pointer type %0; "
               "use array subscript notation instead")
              << LHSType;
        }
      }
    }
    return;
  }

  // Handle compound assignment operators += and -=
  if (const auto *CompOp =
          Result.Nodes.getNodeAs<CompoundAssignOperator>("compoundOp")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(CompOp->getBeginLoc()))
      return;

    QualType LHSType = CompOp->getLHS()->getType();

    if (CompOp->getOpcode() == BO_AddAssign) {
      diag(CompOp->getOperatorLoc(),
           "compound assignment '+=' operator used on pointer type %0; "
           "use array subscript notation instead")
          << LHSType;
    } else if (CompOp->getOpcode() == BO_SubAssign) {
      diag(CompOp->getOperatorLoc(),
           "compound assignment '-=' operator used on pointer type %0; "
           "use array subscript notation instead")
          << LHSType;
    }
  }
}

} // namespace clang::tidy::automotive

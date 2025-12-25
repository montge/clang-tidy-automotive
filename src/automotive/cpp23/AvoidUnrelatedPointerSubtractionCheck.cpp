//===--- AvoidUnrelatedPointerSubtractionCheck.cpp - clang-tidy ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnrelatedPointerSubtractionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidUnrelatedPointerSubtractionCheck::registerMatchers(
    MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match pointer subtraction: ptr1 - ptr2
  Finder->addMatcher(
      binaryOperator(hasOperatorName("-"),
                     hasLHS(expr(hasType(pointerType())).bind("lhs")),
                     hasRHS(expr(hasType(pointerType())).bind("rhs")))
          .bind("subtraction"),
      this);
}

const Expr *
AvoidUnrelatedPointerSubtractionCheck::getPointerOrigin(const Expr *E) const {
  if (!E)
    return nullptr;

  // Iteratively follow through casts and variable references
  // to find the ultimate origin of the pointer
  while (E) {
    // Strip implicit casts and parentheses
    E = E->IgnoreParenImpCasts();

    // Handle explicit casts by looking through them
    if (const auto *Cast = dyn_cast<CXXStaticCastExpr>(E)) {
      E = Cast->getSubExpr();
      continue;
    }
    if (const auto *Cast = dyn_cast<CStyleCastExpr>(E)) {
      E = Cast->getSubExpr();
      continue;
    }

    // Follow variable references to their initializers
    if (const auto *DeclRef = dyn_cast<DeclRefExpr>(E)) {
      if (const auto *Var = dyn_cast<VarDecl>(DeclRef->getDecl())) {
        if (Var->hasInit()) {
          E = Var->getInit();
          continue;
        }
      }
    }

    // No more transformations to apply
    break;
  }

  return E;
}

bool AvoidUnrelatedPointerSubtractionCheck::isDistinctPointerOrigin(
    const Expr *E) const {
  // E should already be the origin expression from getPointerOrigin
  if (!E)
    return false;

  // Check if it's an address-of expression (&var, &arr[i])
  if (isa<UnaryOperator>(E)) {
    const auto *UnOp = cast<UnaryOperator>(E);
    if (UnOp->getOpcode() == UO_AddrOf) {
      return true;
    }
  }

  // Check if it's a new expression
  if (isa<CXXNewExpr>(E)) {
    return true;
  }

  return false;
}

void AvoidUnrelatedPointerSubtractionCheck::check(
    const MatchFinder::MatchResult &Result) {

  const auto *Subtraction =
      Result.Nodes.getNodeAs<BinaryOperator>("subtraction");
  const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
  const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

  if (!Subtraction || !LHS || !RHS)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Subtraction->getOperatorLoc()))
    return;

  // Get the origins of both pointers
  const Expr *LHSOrigin = getPointerOrigin(LHS);
  const Expr *RHSOrigin = getPointerOrigin(RHS);

  if (!LHSOrigin || !RHSOrigin)
    return;

  // Check for obvious violations: different address-of expressions
  bool LHSIsDistinct = isDistinctPointerOrigin(LHSOrigin);
  bool RHSIsDistinct = isDistinctPointerOrigin(RHSOrigin);

  if (LHSIsDistinct && RHSIsDistinct) {
    // Both are distinct pointer origins - check if they're different
    // If both are address-of expressions
    const auto *LHSUnOp = dyn_cast<UnaryOperator>(LHSOrigin);
    const auto *RHSUnOp = dyn_cast<UnaryOperator>(RHSOrigin);

    if (LHSUnOp && RHSUnOp) {
      const Expr *LHSBase = LHSUnOp->getSubExpr()->IgnoreParenImpCasts();
      const Expr *RHSBase = RHSUnOp->getSubExpr()->IgnoreParenImpCasts();

      // Check if they refer to the same base object
      // For simple cases like &a and &b, compare the variable declarations
      const ValueDecl *LHSDecl = nullptr;
      const ValueDecl *RHSDecl = nullptr;

      // Try to extract the base variable/member
      if (const auto *LHSDeclRef = dyn_cast<DeclRefExpr>(LHSBase)) {
        LHSDecl = LHSDeclRef->getDecl();
      } else if (const auto *LHSMember = dyn_cast<MemberExpr>(LHSBase)) {
        LHSDecl = LHSMember->getMemberDecl();
      } else if (const auto *LHSArray = dyn_cast<ArraySubscriptExpr>(LHSBase)) {
        // For array subscripts, get the base array
        if (const auto *BaseDeclRef = dyn_cast<DeclRefExpr>(
                LHSArray->getBase()->IgnoreParenImpCasts())) {
          LHSDecl = BaseDeclRef->getDecl();
        }
      }

      if (const auto *RHSDeclRef = dyn_cast<DeclRefExpr>(RHSBase)) {
        RHSDecl = RHSDeclRef->getDecl();
      } else if (const auto *RHSMember = dyn_cast<MemberExpr>(RHSBase)) {
        RHSDecl = RHSMember->getMemberDecl();
      } else if (const auto *RHSArray = dyn_cast<ArraySubscriptExpr>(RHSBase)) {
        // For array subscripts, get the base array
        if (const auto *BaseDeclRef = dyn_cast<DeclRefExpr>(
                RHSArray->getBase()->IgnoreParenImpCasts())) {
          RHSDecl = BaseDeclRef->getDecl();
        }
      }

      // If we found different base objects, warn
      if (LHSDecl && RHSDecl && LHSDecl != RHSDecl) {
        diag(Subtraction->getOperatorLoc(),
             "pointer subtraction between pointers to different objects '%0' "
             "and '%1' results in undefined behavior")
            << LHSDecl->getName() << RHSDecl->getName();
        return;
      }
    }

    // Check if they're different new expressions
    if (isa<CXXNewExpr>(LHSOrigin) && isa<CXXNewExpr>(RHSOrigin)) {
      // Different new expressions are definitely unrelated
      if (LHSOrigin != RHSOrigin) {
        diag(Subtraction->getOperatorLoc(),
             "pointer subtraction between pointers from different 'new' "
             "expressions results in undefined behavior");
        return;
      }
    }

    // Check if one is address-of and the other is new
    if ((LHSIsDistinct && isa<CXXNewExpr>(LHSOrigin) && RHSIsDistinct &&
         isa<UnaryOperator>(RHSOrigin)) ||
        (RHSIsDistinct && isa<CXXNewExpr>(RHSOrigin) && LHSIsDistinct &&
         isa<UnaryOperator>(LHSOrigin))) {
      diag(Subtraction->getOperatorLoc(),
           "pointer subtraction between 'new' allocation and address-of "
           "expression results in undefined behavior");
      return;
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

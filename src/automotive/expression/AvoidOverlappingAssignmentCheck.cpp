//===--- AvoidOverlappingAssignmentCheck.cpp - clang-tidy ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidOverlappingAssignmentCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidOverlappingAssignmentCheck::registerMatchers(MatchFinder *Finder) {
  // Match assignment operators
  Finder->addMatcher(binaryOperator(isAssignmentOperator()).bind("assign"),
                     this);

  // Match memcpy/memmove calls
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("memcpy", "memmove", "strcpy",
                                                "strncpy", "memset"))))
          .bind("memcall"),
      this);
}

bool AvoidOverlappingAssignmentCheck::mayOverlap(const Expr *LHS,
                                                  const Expr *RHS,
                                                  ASTContext &Context) {
  if (!LHS || !RHS)
    return false;

  LHS = LHS->IgnoreParenImpCasts();
  RHS = RHS->IgnoreParenImpCasts();

  // Check for exact same expression (self-assignment)
  if (LHS == RHS)
    return true;

  // Check if both are references to the same variable
  const auto *LHSDeclRef = dyn_cast<DeclRefExpr>(LHS);
  const auto *RHSDeclRef = dyn_cast<DeclRefExpr>(RHS);

  if (LHSDeclRef && RHSDeclRef) {
    if (LHSDeclRef->getDecl() == RHSDeclRef->getDecl())
      return true;
  }

  // Check for array subscript on same base
  if (const auto *LHSArraySub = dyn_cast<ArraySubscriptExpr>(LHS)) {
    if (const auto *RHSArraySub = dyn_cast<ArraySubscriptExpr>(RHS)) {
      const Expr *LHSBase = LHSArraySub->getBase()->IgnoreParenImpCasts();
      const Expr *RHSBase = RHSArraySub->getBase()->IgnoreParenImpCasts();

      // If same base array, could overlap
      if (const auto *LHSBaseRef = dyn_cast<DeclRefExpr>(LHSBase)) {
        if (const auto *RHSBaseRef = dyn_cast<DeclRefExpr>(RHSBase)) {
          if (LHSBaseRef->getDecl() == RHSBaseRef->getDecl()) {
            // Same array base - could overlap depending on indices
            return true;
          }
        }
      }
    }
  }

  // Check for member access on same object
  if (const auto *LHSMember = dyn_cast<MemberExpr>(LHS)) {
    if (const auto *RHSMember = dyn_cast<MemberExpr>(RHS)) {
      return mayOverlap(LHSMember->getBase(), RHSMember->getBase(), Context);
    }
  }

  return false;
}

void AvoidOverlappingAssignmentCheck::check(
    const MatchFinder::MatchResult &Result) {
  if (const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assign")) {
    if (Result.SourceManager->isInSystemHeader(Assign->getOperatorLoc()))
      return;

    const Expr *LHS = Assign->getLHS();
    const Expr *RHS = Assign->getRHS();

    if (mayOverlap(LHS, RHS, *Result.Context)) {
      diag(Assign->getOperatorLoc(),
           "objects shall not be assigned to an overlapping object");
    }
  } else if (const auto *Call =
                 Result.Nodes.getNodeAs<CallExpr>("memcall")) {
    if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
      return;

    // For memcpy/memmove/strcpy: check if source and destination may overlap
    // Basic check: compare the arguments
    if (Call->getNumArgs() >= 2) {
      const Expr *Dest = Call->getArg(0);
      const Expr *Src = Call->getArg(1);

      if (mayOverlap(Dest, Src, *Result.Context)) {
        const auto *Callee = Call->getDirectCallee();
        if (Callee && Callee->getName() == "memcpy") {
          // memcpy specifically requires non-overlapping regions
          diag(Call->getBeginLoc(),
               "memcpy shall not be used with overlapping objects; use "
               "memmove instead");
        } else {
          diag(Call->getBeginLoc(),
               "objects shall not be assigned to an overlapping object");
        }
      }
    }
  }
}

} // namespace clang::tidy::automotive

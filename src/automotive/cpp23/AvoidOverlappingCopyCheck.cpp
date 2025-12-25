//===--- AvoidOverlappingCopyCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidOverlappingCopyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidOverlappingCopyCheck::registerMatchers(MatchFinder *Finder) {
  // Match copy/move assignment operators where LHS and RHS may be the same
  Finder->addMatcher(cxxOperatorCallExpr(hasOverloadedOperatorName("="),
                                         unless(isExpansionInSystemHeader()),
                                         unless(isInTemplateInstantiation()),
                                         hasArgument(0, expr().bind("lhs")),
                                         hasArgument(1, expr().bind("rhs")))
                         .bind("assignment"),
                     this);

  // Match direct assignment expressions (member access or variable)
  Finder->addMatcher(
      binaryOperator(hasOperatorName("="), unless(isExpansionInSystemHeader()),
                     unless(isInTemplateInstantiation()),
                     hasLHS(expr().bind("lhs")), hasRHS(expr().bind("rhs")))
          .bind("binaryAssignment"),
      this);

  // Match calls to memcpy which should not be used with overlapping ranges
  Finder->addMatcher(callExpr(callee(functionDecl(hasName("::memcpy"))),
                              unless(isExpansionInSystemHeader()),
                              unless(isInTemplateInstantiation()),
                              argumentCountIs(3),
                              hasArgument(0, expr().bind("dest")),
                              hasArgument(1, expr().bind("src")),
                              hasArgument(2, expr().bind("size")))
                         .bind("memcpy"),
                     this);

  // Match calls to std::copy with potentially overlapping ranges
  Finder->addMatcher(callExpr(callee(functionDecl(hasName("::std::copy"))),
                              unless(isExpansionInSystemHeader()),
                              unless(isInTemplateInstantiation()),
                              argumentCountIs(3),
                              hasArgument(0, expr().bind("first")),
                              hasArgument(1, expr().bind("last")),
                              hasArgument(2, expr().bind("destIter")))
                         .bind("stdcopy"),
                     this);

  // Match copy constructor calls where source and destination are the same
  Finder->addMatcher(
      cxxConstructExpr(hasDeclaration(cxxConstructorDecl(isCopyConstructor())),
                       unless(isExpansionInSystemHeader()),
                       unless(isInTemplateInstantiation()), argumentCountIs(1),
                       hasArgument(0, expr().bind("copyCtorArg")))
          .bind("copyConstructor"),
      this);

  // Match move constructor calls where source and destination are the same
  Finder->addMatcher(
      cxxConstructExpr(hasDeclaration(cxxConstructorDecl(isMoveConstructor())),
                       unless(isExpansionInSystemHeader()),
                       unless(isInTemplateInstantiation()), argumentCountIs(1),
                       hasArgument(0, expr().bind("moveCtorArg")))
          .bind("moveConstructor"),
      this);
}

bool AvoidOverlappingCopyCheck::areSameObject(const Expr *E1, const Expr *E2,
                                              const ASTContext &Ctx) const {
  if (!E1 || !E2)
    return false;

  // Strip implicit casts and parentheses
  E1 = E1->IgnoreParenImpCasts();
  E2 = E2->IgnoreParenImpCasts();

  // Check if both refer to the same DeclRefExpr
  if (const auto *DRE1 = dyn_cast<DeclRefExpr>(E1)) {
    if (const auto *DRE2 = dyn_cast<DeclRefExpr>(E2)) {
      return DRE1->getDecl() == DRE2->getDecl();
    }
  }

  // Check if both refer to the same MemberExpr
  if (const auto *ME1 = dyn_cast<MemberExpr>(E1)) {
    if (const auto *ME2 = dyn_cast<MemberExpr>(E2)) {
      if (ME1->getMemberDecl() == ME2->getMemberDecl()) {
        return areSameObject(ME1->getBase(), ME2->getBase(), Ctx);
      }
    }
  }

  // Check if both are 'this' pointer
  if (isa<CXXThisExpr>(E1) && isa<CXXThisExpr>(E2))
    return true;

  // Check if both refer to the same array subscript
  if (const auto *ASE1 = dyn_cast<ArraySubscriptExpr>(E1)) {
    if (const auto *ASE2 = dyn_cast<ArraySubscriptExpr>(E2)) {
      // Check if base arrays are the same
      if (areSameObject(ASE1->getBase(), ASE2->getBase(), Ctx)) {
        // For conservative analysis, if indices are the same syntactically
        const Expr *Idx1 = ASE1->getIdx()->IgnoreParenImpCasts();
        const Expr *Idx2 = ASE2->getIdx()->IgnoreParenImpCasts();

        // Compare integer literals
        if (const auto *IL1 = dyn_cast<IntegerLiteral>(Idx1)) {
          if (const auto *IL2 = dyn_cast<IntegerLiteral>(Idx2)) {
            return IL1->getValue() == IL2->getValue();
          }
        }

        // Same variable index
        if (const auto *DRE1 = dyn_cast<DeclRefExpr>(Idx1)) {
          if (const auto *DRE2 = dyn_cast<DeclRefExpr>(Idx2)) {
            return DRE1->getDecl() == DRE2->getDecl();
          }
        }
      }
    }
  }

  // Check if both are UnaryOperator (e.g., dereference)
  if (const auto *UO1 = dyn_cast<UnaryOperator>(E1)) {
    if (const auto *UO2 = dyn_cast<UnaryOperator>(E2)) {
      if (UO1->getOpcode() == UO2->getOpcode()) {
        return areSameObject(UO1->getSubExpr(), UO2->getSubExpr(), Ctx);
      }
    }
  }

  return false;
}

bool AvoidOverlappingCopyCheck::mayOverlap(const Expr *Dest, const Expr *Src,
                                           const Expr *Size,
                                           const ASTContext &Ctx) const {
  if (!Dest || !Src)
    return false;

  // If they refer to the same base object, they may overlap
  if (areSameObject(Dest, Src, Ctx))
    return true;

  // Check for pointer arithmetic cases
  Dest = Dest->IgnoreParenImpCasts();
  Src = Src->IgnoreParenImpCasts();

  // Check if both are pointer additions from the same base
  const Expr *DestBase = nullptr;
  const Expr *SrcBase = nullptr;

  if (const auto *BO = dyn_cast<BinaryOperator>(Dest)) {
    if (BO->getOpcode() == BO_Add) {
      DestBase = BO->getLHS()->IgnoreParenImpCasts();
    }
  } else {
    DestBase = Dest;
  }

  if (const auto *BO = dyn_cast<BinaryOperator>(Src)) {
    if (BO->getOpcode() == BO_Add) {
      SrcBase = BO->getLHS()->IgnoreParenImpCasts();
    }
  } else {
    SrcBase = Src;
  }

  if (DestBase && SrcBase && areSameObject(DestBase, SrcBase, Ctx))
    return true;

  return false;
}

void AvoidOverlappingCopyCheck::check(const MatchFinder::MatchResult &Result) {

  // Handle operator= calls (copy/move assignment)
  if (const auto *OpCall =
          Result.Nodes.getNodeAs<CXXOperatorCallExpr>("assignment")) {
    const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
    const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

    if (LHS && RHS && areSameObject(LHS, RHS, *Result.Context)) {
      diag(OpCall->getBeginLoc(),
           "self-assignment detected; copy/move operations should not have "
           "overlapping source and destination")
          << OpCall->getSourceRange();
    }
    return;
  }

  // Handle binary assignment (direct =)
  if (const auto *BinOp =
          Result.Nodes.getNodeAs<BinaryOperator>("binaryAssignment")) {
    const auto *LHS = Result.Nodes.getNodeAs<Expr>("lhs");
    const auto *RHS = Result.Nodes.getNodeAs<Expr>("rhs");

    if (LHS && RHS && areSameObject(LHS, RHS, *Result.Context)) {
      // Skip trivial types where self-assignment is harmless
      QualType LHSType = LHS->getType();
      if (LHSType.isTriviallyCopyableType(*Result.Context) &&
          !LHSType->isPointerType() && !LHSType->isArrayType()) {
        return;
      }

      diag(BinOp->getBeginLoc(),
           "self-assignment detected; copy/move operations should not have "
           "overlapping source and destination")
          << BinOp->getSourceRange();
    }
    return;
  }

  // Handle memcpy calls
  if (const auto *Call = Result.Nodes.getNodeAs<CallExpr>("memcpy")) {
    const auto *Dest = Result.Nodes.getNodeAs<Expr>("dest");
    const auto *Src = Result.Nodes.getNodeAs<Expr>("src");
    const auto *Size = Result.Nodes.getNodeAs<Expr>("size");

    if (Dest && Src && mayOverlap(Dest, Src, Size, *Result.Context)) {
      diag(Call->getBeginLoc(),
           "memcpy called with potentially overlapping source and destination; "
           "use memmove for overlapping ranges")
          << Call->getSourceRange();
    }
    return;
  }

  // Handle std::copy calls
  if (const auto *Call = Result.Nodes.getNodeAs<CallExpr>("stdcopy")) {
    const auto *First = Result.Nodes.getNodeAs<Expr>("first");
    const auto *DestIter = Result.Nodes.getNodeAs<Expr>("destIter");

    if (First && DestIter && areSameObject(First, DestIter, *Result.Context)) {
      diag(Call->getBeginLoc(),
           "std::copy called with overlapping source and destination ranges; "
           "behavior is undefined")
          << Call->getSourceRange();
    }
    return;
  }

  // Handle copy constructor calls
  if (const auto *CtorExpr =
          Result.Nodes.getNodeAs<CXXConstructExpr>("copyConstructor")) {
    const auto *Arg = Result.Nodes.getNodeAs<Expr>("copyCtorArg");

    // For copy constructors, check if constructing from self
    // This is rare but can happen in placement new scenarios
    if (Arg) {
      Arg = Arg->IgnoreParenImpCasts();

      // Check for explicit self-copy patterns
      if (const auto *UO = dyn_cast<UnaryOperator>(Arg)) {
        if (UO->getOpcode() == UO_Deref) {
          const Expr *PtrExpr = UO->getSubExpr()->IgnoreParenImpCasts();
          if (isa<CXXThisExpr>(PtrExpr)) {
            diag(CtorExpr->getBeginLoc(),
                 "copy constructor called with self as argument; "
                 "this creates overlapping source and destination")
                << CtorExpr->getSourceRange();
          }
        }
      }
    }
    return;
  }

  // Handle move constructor calls
  if (const auto *CtorExpr =
          Result.Nodes.getNodeAs<CXXConstructExpr>("moveConstructor")) {
    const auto *Arg = Result.Nodes.getNodeAs<Expr>("moveCtorArg");

    if (Arg) {
      Arg = Arg->IgnoreParenImpCasts();

      // Check for self-move patterns
      if (const auto *UO = dyn_cast<UnaryOperator>(Arg)) {
        if (UO->getOpcode() == UO_Deref) {
          const Expr *PtrExpr = UO->getSubExpr()->IgnoreParenImpCasts();
          if (isa<CXXThisExpr>(PtrExpr)) {
            diag(CtorExpr->getBeginLoc(),
                 "move constructor called with self as argument; "
                 "this creates overlapping source and destination")
                << CtorExpr->getSourceRange();
          }
        }
      }
    }
    return;
  }
}

} // namespace clang::tidy::automotive::cpp23

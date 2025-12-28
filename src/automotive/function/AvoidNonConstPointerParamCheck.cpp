//===--- AvoidNonConstPointerParamCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNonConstPointerParamCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Visitor to check if a pointer parameter is modified (written through)
class ParameterModificationVisitor
    : public RecursiveASTVisitor<ParameterModificationVisitor> {
public:
  explicit ParameterModificationVisitor(const ParmVarDecl *Param,
                                        ASTContext *Ctx)
      : Param(Param), Context(Ctx), IsModified(false) {}

  bool VisitUnaryOperator(UnaryOperator *UO) {
    if (IsModified)
      return false;

    // Check for dereference that leads to modification
    if (UO->getOpcode() == UO_Deref) {
      if (isExprReferringToParam(UO->getSubExpr())) {
        // Check if the dereference is being modified
        if (isExprModified(UO)) {
          IsModified = true;
          return false;
        }
      }
    }

    // Check for increment/decrement on dereferenced pointer: (*p)++
    if (UO->isIncrementDecrementOp()) {
      if (auto *SubUO = dyn_cast<UnaryOperator>(UO->getSubExpr()->IgnoreParenImpCasts())) {
        if (SubUO->getOpcode() == UO_Deref &&
            isExprReferringToParam(SubUO->getSubExpr())) {
          IsModified = true;
          return false;
        }
      }
    }

    return true;
  }

  bool VisitMemberExpr(MemberExpr *ME) {
    if (IsModified)
      return false;

    // Check for p->member or (*p).member modification
    const Expr *Base = ME->getBase()->IgnoreParenImpCasts();

    // Handle arrow operator: p->member
    if (ME->isArrow()) {
      if (isExprReferringToParam(Base)) {
        if (isExprModified(ME)) {
          IsModified = true;
          return false;
        }
      }
    }

    return true;
  }

  bool VisitArraySubscriptExpr(ArraySubscriptExpr *ASE) {
    if (IsModified)
      return false;

    // Check for p[i] modification
    const Expr *Base = ASE->getBase()->IgnoreParenImpCasts();
    if (isExprReferringToParam(Base)) {
      if (isExprModified(ASE)) {
        IsModified = true;
        return false;
      }
    }

    return true;
  }

  bool VisitCallExpr(CallExpr *CE) {
    if (IsModified)
      return false;

    // Check if parameter is passed to a function expecting non-const pointer
    const FunctionDecl *Callee = CE->getDirectCallee();
    if (!Callee)
      return true;

    for (unsigned I = 0; I < CE->getNumArgs(); ++I) {
      const Expr *Arg = CE->getArg(I)->IgnoreParenImpCasts();
      if (isExprReferringToParam(Arg)) {
        if (I < Callee->getNumParams()) {
          QualType ParamType = Callee->getParamDecl(I)->getType();
          // If passed to non-const pointer parameter, assume it may be modified
          if (ParamType->isPointerType() &&
              !ParamType->getPointeeType().isConstQualified()) {
            IsModified = true;
            return false;
          }
        }
      }
    }

    return true;
  }

  bool isModified() const { return IsModified; }

private:
  const ParmVarDecl *Param;
  ASTContext *Context;
  bool IsModified;

  /// Check if expression refers to the tracked parameter
  bool isExprReferringToParam(const Expr *E) {
    E = E->IgnoreParenImpCasts();
    if (const auto *DRE = dyn_cast<DeclRefExpr>(E)) {
      return DRE->getDecl() == Param;
    }
    return false;
  }

  /// Check if an expression is being modified (is LHS of assignment, etc.)
  bool isExprModified(const Expr *E) {
    const auto &Parents = Context->getParents(*E);
    for (const auto &Parent : Parents) {
      // Direct assignment: *p = value or p->member = value
      if (const auto *BO = Parent.get<BinaryOperator>()) {
        if (BO->isAssignmentOp() &&
            BO->getLHS()->IgnoreParenImpCasts() == E) {
          return true;
        }
      }

      // Compound assignment: *p += value
      if (const auto *CAO = Parent.get<CompoundAssignOperator>()) {
        if (CAO->getLHS()->IgnoreParenImpCasts() == E) {
          return true;
        }
      }

      // Increment/decrement: (*p)++ or ++(*p)
      if (const auto *UO = Parent.get<UnaryOperator>()) {
        if (UO->isIncrementDecrementOp()) {
          return true;
        }
        // Address-of could allow modification: &(*p) or &(p->member)
        if (UO->getOpcode() == UO_AddrOf) {
          return true; // Conservative: assume address taken means may be modified
        }
      }
    }
    return false;
  }
};

} // namespace

void AvoidNonConstPointerParamCheck::registerMatchers(MatchFinder *Finder) {
  // Match function definitions with pointer parameters
  Finder->addMatcher(
      functionDecl(
          unless(isExpansionInSystemHeader()),
          isDefinition(),
          hasAnyParameter(
              parmVarDecl(
                  hasType(pointerType(unless(pointee(isConstQualified())))))
                  .bind("param")))
          .bind("func"),
      this);
}

void AvoidNonConstPointerParamCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Param = Result.Nodes.getNodeAs<ParmVarDecl>("param");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

  if (!Param || !Func)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Param->getLocation()))
    return;

  // Skip if parameter type is already const-qualified at pointee level
  QualType ParamType = Param->getType();
  if (!ParamType->isPointerType())
    return;

  QualType PointeeType = ParamType->getPointeeType();
  if (PointeeType.isConstQualified())
    return; // Already const-qualified

  // Skip void pointers (often used for generic interfaces)
  if (PointeeType->isVoidType())
    return;

  // Skip function pointers
  if (PointeeType->isFunctionType())
    return;

  // Check if the parameter is modified
  if (!Func->hasBody())
    return; // Can't check without body

  ParameterModificationVisitor Visitor(Param, Result.Context);
  Visitor.TraverseStmt(Func->getBody());

  if (!Visitor.isModified()) {
    diag(Param->getLocation(),
         "pointer parameter '%0' should be declared const as it is not modified")
        << Param->getName();
  }
}

} // namespace clang::tidy::automotive

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

/// Visitor to check if a parameter is modified
class ParameterModificationVisitor
    : public RecursiveASTVisitor<ParameterModificationVisitor> {
public:
  explicit ParameterModificationVisitor(const ParmVarDecl *Param)
      : Param(Param), IsModified(false) {}

  bool VisitDeclRefExpr(const DeclRefExpr *DRE) {
    if (DRE->getDecl() != Param)
      return true;

    // Check if this reference is on the left side of an assignment
    const auto &Parents = Context->getParents(*DRE);
    for (const auto &Parent : Parents) {
      // Check for assignment operators
      if (const auto *BinOp = Parent.get<BinaryOperator>()) {
        if (BinOp->isAssignmentOp() && BinOp->getLHS()->IgnoreParenImpCasts() == DRE) {
          IsModified = true;
          return false;
        }
      }

      // Check for increment/decrement
      if (const auto *UnOp = Parent.get<UnaryOperator>()) {
        if (UnOp->isIncrementDecrementOp()) {
          IsModified = true;
          return false;
        }
      }

      // Check for compound assignment
      if (const auto *CompoundAssign = Parent.get<CompoundAssignOperator>()) {
        if (CompoundAssign->getLHS()->IgnoreParenImpCasts() == DRE) {
          IsModified = true;
          return false;
        }
      }

      // Check if pointer is dereferenced and modified
      if (const auto *UOp = Parent.get<UnaryOperator>()) {
        if (UOp->getOpcode() == UO_Deref) {
          // Check if the dereferenced value is modified
          const auto &DerefParents = Context->getParents(*UOp);
          for (const auto &DerefParent : DerefParents) {
            if (const auto *DerefBinOp = DerefParent.get<BinaryOperator>()) {
              if (DerefBinOp->isAssignmentOp() &&
                  DerefBinOp->getLHS()->IgnoreParenImpCasts() == UOp) {
                IsModified = true;
                return false;
              }
            }
          }
        }
      }

      // Check if passed to a function that takes non-const pointer
      if (const auto *Call = Parent.get<CallExpr>()) {
        if (const FunctionDecl *Callee = Call->getDirectCallee()) {
          for (unsigned I = 0; I < Call->getNumArgs(); ++I) {
            if (Call->getArg(I)->IgnoreParenImpCasts() == DRE) {
              if (I < Callee->getNumParams()) {
                QualType ParamType = Callee->getParamDecl(I)->getType();
                // If the parameter is a non-const pointer, it might be modified
                if (ParamType->isPointerType() &&
                    !ParamType->getPointeeType().isConstQualified()) {
                  IsModified = true;
                  return false;
                }
              }
            }
          }
        }
      }
    }

    return true;
  }

  bool isModified() const { return IsModified; }

  void setContext(ASTContext *Ctx) { Context = Ctx; }

private:
  const ParmVarDecl *Param;
  bool IsModified;
  ASTContext *Context = nullptr;
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

bool AvoidNonConstPointerParamCheck::isParameterModified(
    const ParmVarDecl *Param, const FunctionDecl *Func) const {
  if (!Func->hasBody())
    return true; // Assume modified if no body

  ParameterModificationVisitor Visitor(Param);
  Visitor.setContext(&Func->getASTContext());
  Visitor.TraverseStmt(Func->getBody());

  return Visitor.isModified();
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
  if (!isParameterModified(Param, Func)) {
    diag(Param->getLocation(),
         "pointer parameter '%0' should be declared const as it is not modified")
        << Param->getName();
  }
}

} // namespace clang::tidy::automotive

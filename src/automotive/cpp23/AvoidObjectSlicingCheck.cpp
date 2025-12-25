//===--- AvoidObjectSlicingCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidObjectSlicingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {
/// Helper function to check if slicing occurs between derived and base classes.
/// \param DerivedDecl The derived class declaration.
/// \param BaseDecl The base class declaration.
/// \return true if DerivedDecl is derived from BaseDecl and they are different.
bool isSlicing(const CXXRecordDecl *DerivedDecl,
               const CXXRecordDecl *BaseDecl) {
  if (!DerivedDecl || !BaseDecl)
    return false;

  // No slicing if they are the same type
  if (DerivedDecl == BaseDecl)
    return false;

  // Check if DerivedDecl is actually derived from BaseDecl
  return DerivedDecl->isDerivedFrom(BaseDecl);
}

/// Extract the CXXRecordDecl from a QualType, handling references and pointers.
/// \param Type The type to extract from.
/// \return The CXXRecordDecl if found, nullptr otherwise.
const CXXRecordDecl *getCXXRecordDeclFromType(QualType Type) {
  // Remove references and qualifiers
  Type = Type.getNonReferenceType().getUnqualifiedType();

  // Get the record type
  if (const auto *RT = Type->getAs<RecordType>())
    return dyn_cast<CXXRecordDecl>(RT->getDecl());

  return nullptr;
}

} // anonymous namespace

void AvoidObjectSlicingCheck::registerMatchers(MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match copy constructor calls where object slicing may occur
  // This happens when a derived class object is used to construct a base class
  Finder->addMatcher(
      cxxConstructExpr(hasDeclaration(cxxConstructorDecl(isCopyConstructor())),
                       hasArgument(0, expr().bind("source-expr")))
          .bind("construct-expr"),
      this);

  // Match copy assignment operations where slicing may occur
  Finder->addMatcher(
      cxxOperatorCallExpr(hasOverloadedOperatorName("="),
                          callee(cxxMethodDecl(isCopyAssignmentOperator())),
                          hasArgument(1, expr().bind("assign-source")))
          .bind("assign-expr"),
      this);

  // Match variable declarations with initializers that may slice
  Finder->addMatcher(varDecl(hasType(cxxRecordDecl().bind("var-base-type")),
                             hasInitializer(expr().bind("var-init-expr")))
                         .bind("var-decl"),
                     this);

  // Match return statements that return by value and may slice
  Finder->addMatcher(returnStmt(hasReturnValue(expr().bind("return-expr")))
                         .bind("return-stmt"),
                     this);
}

void AvoidObjectSlicingCheck::check(const MatchFinder::MatchResult &Result) {
  const SourceManager *SM = Result.SourceManager;

  // Check copy constructor slicing
  if (const auto *ConstructExpr =
          Result.Nodes.getNodeAs<CXXConstructExpr>("construct-expr")) {

    if (SM->isInSystemHeader(ConstructExpr->getBeginLoc()))
      return;

    const auto *SourceExpr = Result.Nodes.getNodeAs<Expr>("source-expr");
    if (!SourceExpr)
      return;

    // Get the type being constructed (base type)
    const CXXRecordDecl *BaseDecl =
        getCXXRecordDeclFromType(ConstructExpr->getType());

    // Get the type of the source expression (potentially derived type)
    const CXXRecordDecl *DerivedDecl =
        getCXXRecordDeclFromType(SourceExpr->getType());

    if (isSlicing(DerivedDecl, BaseDecl)) {
      diag(ConstructExpr->getBeginLoc(),
           "object slicing occurs when copying derived class '%0' to base "
           "class '%1'")
          << DerivedDecl->getQualifiedNameAsString()
          << BaseDecl->getQualifiedNameAsString();
    }
    return;
  }

  // Check copy assignment slicing
  if (const auto *AssignExpr =
          Result.Nodes.getNodeAs<CXXOperatorCallExpr>("assign-expr")) {

    if (SM->isInSystemHeader(AssignExpr->getBeginLoc()))
      return;

    const auto *SourceExpr = Result.Nodes.getNodeAs<Expr>("assign-source");
    if (!SourceExpr)
      return;

    // Get the type being assigned to (base type)
    const CXXRecordDecl *BaseDecl =
        getCXXRecordDeclFromType(AssignExpr->getArg(0)->getType());

    // Get the type of the source expression (potentially derived type)
    const CXXRecordDecl *DerivedDecl =
        getCXXRecordDeclFromType(SourceExpr->getType());

    if (isSlicing(DerivedDecl, BaseDecl)) {
      diag(AssignExpr->getBeginLoc(),
           "object slicing occurs when assigning derived class '%0' to base "
           "class '%1'")
          << DerivedDecl->getQualifiedNameAsString()
          << BaseDecl->getQualifiedNameAsString();
    }
    return;
  }

  // Check variable declaration slicing
  if (const auto *VarD = Result.Nodes.getNodeAs<VarDecl>("var-decl")) {

    if (SM->isInSystemHeader(VarD->getBeginLoc()))
      return;

    const auto *BaseDecl =
        Result.Nodes.getNodeAs<CXXRecordDecl>("var-base-type");
    const auto *InitExpr = Result.Nodes.getNodeAs<Expr>("var-init-expr");

    if (!BaseDecl || !InitExpr)
      return;

    const CXXRecordDecl *DerivedDecl =
        getCXXRecordDeclFromType(InitExpr->getType());

    if (isSlicing(DerivedDecl, BaseDecl)) {
      diag(VarD->getBeginLoc(),
           "object slicing occurs when initializing base class '%1' variable "
           "with derived class '%0'")
          << DerivedDecl->getQualifiedNameAsString()
          << BaseDecl->getQualifiedNameAsString();
    }
    return;
  }

  // Check return statement slicing
  if (const auto *RetStmt = Result.Nodes.getNodeAs<ReturnStmt>("return-stmt")) {

    if (SM->isInSystemHeader(RetStmt->getBeginLoc()))
      return;

    const auto *ReturnExpr = Result.Nodes.getNodeAs<Expr>("return-expr");
    if (!ReturnExpr)
      return;

    // Find the enclosing function to get the return type
    const auto &Parents = Result.Context->getParents(*RetStmt);
    if (Parents.empty())
      return;

    const FunctionDecl *FuncDecl = nullptr;
    for (const auto &Parent : Parents) {
      FuncDecl = Parent.get<FunctionDecl>();
      if (FuncDecl)
        break;
    }

    // If not directly found, traverse up the AST
    if (!FuncDecl) {
      auto CurrentParents = Parents;
      while (!CurrentParents.empty() && !FuncDecl) {
        for (const auto &P : CurrentParents) {
          FuncDecl = P.get<FunctionDecl>();
          if (FuncDecl)
            break;
        }
        if (!FuncDecl && !CurrentParents.empty()) {
          CurrentParents = Result.Context->getParents(CurrentParents[0]);
        }
      }
    }

    if (!FuncDecl)
      return;

    // Get the return type of the function (base type)
    QualType ReturnType = FuncDecl->getReturnType();
    const CXXRecordDecl *BaseDecl = getCXXRecordDeclFromType(ReturnType);

    // Get the type being returned (potentially derived type)
    const CXXRecordDecl *DerivedDecl =
        getCXXRecordDeclFromType(ReturnExpr->getType());

    if (isSlicing(DerivedDecl, BaseDecl)) {
      diag(RetStmt->getBeginLoc(),
           "object slicing occurs when returning derived class '%0' as base "
           "class '%1' by value")
          << DerivedDecl->getQualifiedNameAsString()
          << BaseDecl->getQualifiedNameAsString();
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

//===--- AvoidSlicingCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSlicingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidSlicingCheck::registerMatchers(MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match copy constructor calls where object slicing occurs
  // This happens when a derived class object is used to construct a base class
  Finder->addMatcher(
      cxxConstructExpr(
          hasDeclaration(cxxConstructorDecl(isCopyConstructor())),
          hasArgument(0, expr(hasType(cxxRecordDecl().bind("derived")))))
          .bind("construct"),
      this);

  // Match variable declarations that slice
  Finder->addMatcher(varDecl(hasType(cxxRecordDecl().bind("base-var")),
                             hasInitializer(expr(
                                 hasType(cxxRecordDecl().bind("derived-var")))))
                         .bind("var-decl"),
                     this);
}

void AvoidSlicingCheck::check(const MatchFinder::MatchResult &Result) {
  // Check constructor-based slicing
  if (const auto *Construct =
          Result.Nodes.getNodeAs<CXXConstructExpr>("construct")) {
    if (Result.SourceManager->isInSystemHeader(Construct->getBeginLoc()))
      return;

    const auto *DerivedDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("derived");
    if (!DerivedDecl)
      return;

    // Get the type being constructed
    QualType ConstructedType = Construct->getType();
    const auto *BaseDecl = ConstructedType->getAsCXXRecordDecl();
    if (!BaseDecl || !DerivedDecl)
      return;

    // Check if derived is actually derived from base
    if (DerivedDecl != BaseDecl && DerivedDecl->isDerivedFrom(BaseDecl)) {
      diag(Construct->getBeginLoc(),
           "object slicing occurs when copying derived class '%0' to base "
           "class '%1'")
          << DerivedDecl->getQualifiedNameAsString()
          << BaseDecl->getQualifiedNameAsString();
    }
  }

  // Check variable declaration slicing
  if (const auto *VarD = Result.Nodes.getNodeAs<VarDecl>("var-decl")) {
    if (Result.SourceManager->isInSystemHeader(VarD->getBeginLoc()))
      return;

    const auto *BaseDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("base-var");
    const auto *DerivedDecl =
        Result.Nodes.getNodeAs<CXXRecordDecl>("derived-var");

    if (!BaseDecl || !DerivedDecl)
      return;

    // Check if derived is actually derived from base
    if (DerivedDecl != BaseDecl && DerivedDecl->isDerivedFrom(BaseDecl)) {
      diag(VarD->getBeginLoc(),
           "object slicing occurs when assigning derived class '%0' to base "
           "class '%1' variable")
          << DerivedDecl->getQualifiedNameAsString()
          << BaseDecl->getQualifiedNameAsString();
    }
  }
}

} // namespace clang::tidy::automotive

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

/// Check if slicing occurs between derived and base classes.
static bool isSlicing(const CXXRecordDecl *DerivedDecl,
                      const CXXRecordDecl *BaseDecl) {
  if (!DerivedDecl || !BaseDecl)
    return false;
  return DerivedDecl != BaseDecl && DerivedDecl->isDerivedFrom(BaseDecl);
}

void AvoidSlicingCheck::check(const MatchFinder::MatchResult &Result) {
  // Check constructor-based slicing
  if (const auto *Construct =
          Result.Nodes.getNodeAs<CXXConstructExpr>("construct")) {
    if (Result.SourceManager->isInSystemHeader(Construct->getBeginLoc()))
      return;

    const auto *DerivedDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("derived");
    const auto *BaseDecl = Construct->getType()->getAsCXXRecordDecl();

    if (isSlicing(DerivedDecl, BaseDecl))
      diag(Construct->getBeginLoc(),
           "object slicing occurs when copying derived class '%0' to base "
           "class '%1'")
          << DerivedDecl->getQualifiedNameAsString()
          << BaseDecl->getQualifiedNameAsString();
    return;
  }

  // Check variable declaration slicing
  const auto *VarD = Result.Nodes.getNodeAs<VarDecl>("var-decl");
  if (!VarD)
    return;

  if (Result.SourceManager->isInSystemHeader(VarD->getBeginLoc()))
    return;

  const auto *BaseDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("base-var");
  const auto *DerivedDecl =
      Result.Nodes.getNodeAs<CXXRecordDecl>("derived-var");

  if (isSlicing(DerivedDecl, BaseDecl))
    diag(VarD->getBeginLoc(),
         "object slicing occurs when assigning derived class '%0' to base "
         "class '%1' variable")
        << DerivedDecl->getQualifiedNameAsString()
        << BaseDecl->getQualifiedNameAsString();
}

} // namespace clang::tidy::automotive

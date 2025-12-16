//===--- VirtualDestructorCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "VirtualDestructorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <algorithm>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void VirtualDestructorCheck::registerMatchers(MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match classes that have at least one virtual method
  Finder->addMatcher(cxxRecordDecl(isClass(),
                                   hasMethod(cxxMethodDecl(isVirtual())),
                                   unless(isLambda()))
                         .bind("class"),
                     this);
}

/// Check if any base class has a virtual destructor.
static bool hasVirtualDestructorInBase(const CXXRecordDecl *ClassDecl) {
  return std::any_of(ClassDecl->bases().begin(), ClassDecl->bases().end(),
                     [](const CXXBaseSpecifier &Base) {
                       const auto *BaseClass =
                           Base.getType()->getAsCXXRecordDecl();
                       if (!BaseClass || !BaseClass->hasDefinition())
                         return false;
                       const CXXDestructorDecl *BaseDtor =
                           BaseClass->getDestructor();
                       return BaseDtor && BaseDtor->isVirtual();
                     });
}

void VirtualDestructorCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ClassDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("class");
  if (!ClassDecl || !ClassDecl->hasDefinition())
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(ClassDecl->getBeginLoc()))
    return;

  // Get the destructor
  const CXXDestructorDecl *Destructor = ClassDecl->getDestructor();

  // If there's no destructor or it's implicit, check if any base has virtual
  // destructor
  if (!Destructor || Destructor->isImplicit()) {
    if (!hasVirtualDestructorInBase(ClassDecl))
      diag(ClassDecl->getLocation(),
           "class '%0' has virtual functions but no explicit virtual "
           "destructor")
          << ClassDecl->getQualifiedNameAsString();
    return;
  }

  // Check if the explicit destructor is virtual
  if (!Destructor->isVirtual())
    diag(Destructor->getLocation(),
         "class '%0' has virtual functions but destructor is not virtual")
        << ClassDecl->getQualifiedNameAsString();
}

} // namespace clang::tidy::automotive

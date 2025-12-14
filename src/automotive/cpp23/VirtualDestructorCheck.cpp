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

void VirtualDestructorCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ClassDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("class");
  if (!ClassDecl || !ClassDecl->hasDefinition())
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(ClassDecl->getBeginLoc()))
    return;

  // Get the destructor
  const CXXDestructorDecl *Destructor = ClassDecl->getDestructor();

  // If there's no explicit destructor, check if it's implicitly declared
  if (!Destructor) {
    // No destructor means it's using the default, which is not virtual
    // unless a base class has a virtual destructor
    bool HasVirtualDtorInBase = false;
    for (const auto &Base : ClassDecl->bases()) {
      if (const auto *BaseClass = Base.getType()->getAsCXXRecordDecl()) {
        if (BaseClass->hasDefinition()) {
          const CXXDestructorDecl *BaseDtor = BaseClass->getDestructor();
          if (BaseDtor && BaseDtor->isVirtual()) {
            HasVirtualDtorInBase = true;
            break;
          }
        }
      }
    }

    if (!HasVirtualDtorInBase) {
      diag(ClassDecl->getLocation(),
           "class '%0' has virtual functions but no explicit virtual "
           "destructor")
          << ClassDecl->getQualifiedNameAsString();
    }
    return;
  }

  // Check if the destructor is virtual
  if (!Destructor->isVirtual()) {
    diag(Destructor->getLocation(),
         "class '%0' has virtual functions but destructor is not virtual")
        << ClassDecl->getQualifiedNameAsString();
  }
}

} // namespace clang::tidy::automotive

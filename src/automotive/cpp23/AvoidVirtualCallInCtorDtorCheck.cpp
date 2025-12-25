//===--- AvoidVirtualCallInCtorDtorCheck.cpp - clang-tidy ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidVirtualCallInCtorDtorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidVirtualCallInCtorDtorCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match member call expressions within constructors or destructors
  // We need to check if the called method is virtual
  Finder->addMatcher(
      cxxMemberCallExpr(
          hasAncestor(cxxConstructorDecl(unless(isImplicit())).bind("ctor")))
          .bind("call"),
      this);

  Finder->addMatcher(
      cxxMemberCallExpr(
          hasAncestor(cxxDestructorDecl(unless(isImplicit())).bind("dtor")))
          .bind("call"),
      this);
}

void AvoidVirtualCallInCtorDtorCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CXXMemberCallExpr>("call");
  if (!Call)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  // Get the method being called
  const CXXMethodDecl *Method = Call->getMethodDecl();
  if (!Method)
    return;

  // Check if the method is virtual
  if (!Method->isVirtual())
    return;

  // Determine whether we're in a constructor or destructor
  const CXXConstructorDecl *Ctor =
      Result.Nodes.getNodeAs<CXXConstructorDecl>("ctor");
  const CXXDestructorDecl *Dtor =
      Result.Nodes.getNodeAs<CXXDestructorDecl>("dtor");

  // Check if this is a direct call (member of 'this')
  // We want to avoid flagging calls on other objects
  if (const Expr *Base = Call->getImplicitObjectArgument()) {
    // Check if the call is on 'this' (explicitly or implicitly)
    if (const auto *ICE = dyn_cast<ImplicitCastExpr>(Base)) {
      if (const auto *CTE = dyn_cast<CXXThisExpr>(ICE->getSubExpr())) {
        // This is a call on 'this' - report it
        if (Ctor) {
          diag(Call->getBeginLoc(),
               "virtual function '%0' called in constructor '%1'")
              << Method->getQualifiedNameAsString()
              << Ctor->getParent()->getQualifiedNameAsString();
          diag(Method->getLocation(), "virtual function declared here",
               DiagnosticIDs::Note);
        } else if (Dtor) {
          diag(Call->getBeginLoc(),
               "virtual function '%0' called in destructor '%1'")
              << Method->getQualifiedNameAsString()
              << Dtor->getParent()->getQualifiedNameAsString();
          diag(Method->getLocation(), "virtual function declared here",
               DiagnosticIDs::Note);
        }
        return;
      }
    }

    // Also check for direct CXXThisExpr (less common)
    if (isa<CXXThisExpr>(Base)) {
      if (Ctor) {
        diag(Call->getBeginLoc(),
             "virtual function '%0' called in constructor '%1'")
            << Method->getQualifiedNameAsString()
            << Ctor->getParent()->getQualifiedNameAsString();
        diag(Method->getLocation(), "virtual function declared here",
             DiagnosticIDs::Note);
      } else if (Dtor) {
        diag(Call->getBeginLoc(),
             "virtual function '%0' called in destructor '%1'")
            << Method->getQualifiedNameAsString()
            << Dtor->getParent()->getQualifiedNameAsString();
        diag(Method->getLocation(), "virtual function declared here",
             DiagnosticIDs::Note);
      }
      return;
    }
  }

  // If there's no explicit base, it's an implicit call on 'this'
  // This handles cases like: virtualMethod() instead of this->virtualMethod()
  if (!Call->getImplicitObjectArgument() ||
      isa<CXXThisExpr>(Call->getImplicitObjectArgument()->IgnoreImpCasts())) {
    if (Ctor) {
      diag(Call->getBeginLoc(),
           "virtual function '%0' called in constructor '%1'")
          << Method->getQualifiedNameAsString()
          << Ctor->getParent()->getQualifiedNameAsString();
      diag(Method->getLocation(), "virtual function declared here",
           DiagnosticIDs::Note);
    } else if (Dtor) {
      diag(Call->getBeginLoc(),
           "virtual function '%0' called in destructor '%1'")
          << Method->getQualifiedNameAsString()
          << Dtor->getParent()->getQualifiedNameAsString();
      diag(Method->getLocation(), "virtual function declared here",
           DiagnosticIDs::Note);
    }
  }
}

} // namespace clang::tidy::automotive

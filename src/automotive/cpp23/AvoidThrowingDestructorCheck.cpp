//===--- AvoidThrowingDestructorCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidThrowingDestructorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidThrowingDestructorCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match throw expressions inside destructors
  Finder->addMatcher(
      cxxThrowExpr(hasAncestor(cxxDestructorDecl().bind("destructor")))
          .bind("throw"),
      this);

  // Also match destructors that are not noexcept (in C++11 and later,
  // destructors should be implicitly noexcept)
  Finder->addMatcher(
      cxxDestructorDecl(unless(isNoThrow())).bind("non_noexcept_dtor"), this);
}

void AvoidThrowingDestructorCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Check for throw expressions in destructors
  if (const auto *Throw = Result.Nodes.getNodeAs<CXXThrowExpr>("throw")) {
    const auto *Dtor = Result.Nodes.getNodeAs<CXXDestructorDecl>("destructor");

    if (Result.SourceManager->isInSystemHeader(Throw->getThrowLoc()))
      return;

    diag(Throw->getThrowLoc(),
         "throw expression in destructor %0; destructors should not throw "
         "exceptions")
        << Dtor;
    return;
  }

  // Check for non-noexcept destructors
  if (const auto *Dtor =
          Result.Nodes.getNodeAs<CXXDestructorDecl>("non_noexcept_dtor")) {
    if (Result.SourceManager->isInSystemHeader(Dtor->getLocation()))
      return;

    // Skip if destructor is defaulted or deleted
    if (Dtor->isDefaulted() || Dtor->isDeleted())
      return;

    // Skip if destructor is implicitly generated
    if (Dtor->isImplicit())
      return;

    // In C++11 and later, user-declared destructors should be noexcept
    // unless they explicitly specify otherwise
    const auto *ExceptionSpec = Dtor->getType()->getAs<FunctionProtoType>();
    // LCOV_EXCL_START - defensive check, destructor always has prototype
    if (!ExceptionSpec)
      return;
    // LCOV_EXCL_STOP

    // If the destructor has an explicit non-noexcept specification, warn
    if (ExceptionSpec->hasExceptionSpec() &&
        ExceptionSpec->getExceptionSpecType() != EST_NoThrow &&
        ExceptionSpec->getExceptionSpecType() != EST_BasicNoexcept &&
        ExceptionSpec->getExceptionSpecType() != EST_NoexceptTrue) {
      diag(Dtor->getLocation(),
           "destructor %0 has exception specification that may throw; "
           "destructors should be noexcept")
          << Dtor;
    }
  }
}

} // namespace clang::tidy::automotive

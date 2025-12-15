//===--- AvoidPointerTypedefCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPointerTypedefCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPointerTypedefCheck::registerMatchers(MatchFinder *Finder) {
  // Match typedef declarations
  Finder->addMatcher(typedefDecl().bind("typedef"), this);

  // Also match type alias declarations (C++11 using)
  Finder->addMatcher(typeAliasDecl().bind("alias"), this);
}

void AvoidPointerTypedefCheck::check(const MatchFinder::MatchResult &Result) {
  // Check typedef declarations
  if (const auto *TD = Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
    // Skip built-in types and system headers
    if (!TD->getLocation().isValid() ||
        Result.SourceManager->isInSystemHeader(TD->getLocation()))
      return;

    QualType UnderlyingType = TD->getUnderlyingType();

    // Check if the underlying type is a pointer type
    if (UnderlyingType->isPointerType()) {
      // Skip function pointers - these are commonly typedef'd and acceptable
      if (UnderlyingType->getPointeeType()->isFunctionType())
        return;

      diag(TD->getLocation(),
           "typedef %0 hides pointer type; consider making the pointer "
           "explicit at point of use")
          << TD;
    }
    return;
  }

  // Check type alias declarations
  if (const auto *TAD = Result.Nodes.getNodeAs<TypeAliasDecl>("alias")) {
    // Skip built-in types and system headers
    if (!TAD->getLocation().isValid() ||
        Result.SourceManager->isInSystemHeader(TAD->getLocation()))
      return;

    QualType UnderlyingType = TAD->getUnderlyingType();

    // Check if the underlying type is a pointer type
    if (UnderlyingType->isPointerType()) {
      // Skip function pointers
      if (UnderlyingType->getPointeeType()->isFunctionType())
        return;

      diag(TAD->getLocation(),
           "type alias %0 hides pointer type; consider making the pointer "
           "explicit at point of use")
          << TAD;
    }
  }
}

} // namespace clang::tidy::automotive

//===--- ExplicitEnumeratorValuesCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExplicitEnumeratorValuesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ExplicitEnumeratorValuesCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(enumDecl().bind("enum"), this);
}

void ExplicitEnumeratorValuesCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *ED = Result.Nodes.getNodeAs<EnumDecl>("enum");
  if (!ED)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(ED->getLocation()))
    return;

  // Skip forward declarations
  if (!ED->isCompleteDefinition())
    return;

  // Check each enumerator for an explicit initializer
  for (const auto *Enumerator : ED->enumerators()) {
    // getInitExpr() returns null if no explicit initializer was provided
    if (!Enumerator->getInitExpr()) {
      diag(Enumerator->getLocation(),
           "enumerator %0 has implicit value; each enumerator should have an "
           "explicit value")
          << Enumerator;
    }
  }
}

} // namespace clang::tidy::automotive

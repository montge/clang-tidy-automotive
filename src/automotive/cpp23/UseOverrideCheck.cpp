//===--- UseOverrideCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseOverrideCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void UseOverrideCheck::registerMatchers(MatchFinder *Finder) {
  // Only match in C++11 and later (when override keyword was introduced)
  if (!getLangOpts().CPlusPlus11)
    return;

  // Match CXX method declarations that override a base class method
  Finder->addMatcher(cxxMethodDecl(isOverride()).bind("method"), this);
}

void UseOverrideCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Method = Result.Nodes.getNodeAs<CXXMethodDecl>("method");
  if (!Method)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Method->getLocation()))
    return;

  // Skip if this is the first declaration of a virtual function
  // (i.e., it's not overriding anything from a base class)
  if (Method->size_overridden_methods() == 0)
    return;

  // Check if the method already has the override or final specifier
  bool HasOverride = Method->getAttr<OverrideAttr>();
  bool HasFinal = Method->getAttr<FinalAttr>();

  // If it already has override or final, no need to warn
  if (HasOverride || HasFinal)
    return;

  diag(Method->getLocation(),
       "overriding function '%0' should be marked with 'override' specifier")
      << Method->getQualifiedNameAsString();
}

} // namespace clang::tidy::automotive::cpp23

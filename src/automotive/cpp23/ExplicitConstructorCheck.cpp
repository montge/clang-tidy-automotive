//===--- ExplicitConstructorCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExplicitConstructorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ExplicitConstructorCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match non-explicit constructors
  Finder->addMatcher(cxxConstructorDecl(unless(isExplicit()),
                                        unless(isImplicit()),
                                        unless(isCopyConstructor()),
                                        unless(isMoveConstructor()))
                         .bind("ctor"),
                     this);
}

void ExplicitConstructorCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Ctor = Result.Nodes.getNodeAs<CXXConstructorDecl>("ctor");
  if (!Ctor)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Ctor->getLocation()))
    return;

  // Skip deleted constructors
  if (Ctor->isDeleted())
    return;

  // Skip default constructors (no parameters)
  if (Ctor->getNumParams() == 0)
    return;

  // Count required parameters (those without defaults)
  unsigned RequiredParams = 0;
  for (const auto *Param : Ctor->parameters()) {
    if (!Param->hasDefaultArg())
      RequiredParams++;
  }

  // Check if constructor can be called with a single argument
  bool CanBeCalledWithSingleArg =
      (Ctor->getNumParams() == 1) ||
      (RequiredParams <= 1 && Ctor->getNumParams() > 0);

  if (!CanBeCalledWithSingleArg)
    return;

  // Get the parameter type for the message
  QualType ParamType = Ctor->getParamDecl(0)->getType();

  // Skip if the parameter type is the same as or derived from the class type
  const CXXRecordDecl *ParentClass = Ctor->getParent();
  QualType ClassType =
      Result.Context->getRecordType(ParentClass).getCanonicalType();
  QualType CanonParamType = ParamType.getNonReferenceType().getCanonicalType();

  // Allow implicit copy/move from same type
  if (CanonParamType == ClassType)
    return;

  diag(Ctor->getLocation(),
       "single-argument constructor should be marked explicit to prevent "
       "implicit conversions from %0")
      << ParamType;
}

} // namespace clang::tidy::automotive

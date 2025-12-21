//===--- AvoidQualifiedFunctionTypeCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidQualifiedFunctionTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidQualifiedFunctionTypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl().bind("varDecl"), this);
  Finder->addMatcher(typedefDecl().bind("typedefDecl"), this);
}

void AvoidQualifiedFunctionTypeCheck::check(
    const MatchFinder::MatchResult &Result) {
  SourceLocation Loc;
  QualType Type;

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;
    Loc = VD->getLocation();
    Type = VD->getType();
  } else if (const auto *TD =
                 Result.Nodes.getNodeAs<TypedefDecl>("typedefDecl")) {
    if (Result.SourceManager->isInSystemHeader(TD->getLocation()))
      return;
    Loc = TD->getLocation();
    Type = TD->getUnderlyingType();
  } else {
    return;
  }

  // Check if the type is a qualified function type
  // Need to look through any typedef/sugar to find if there's a function type
  // that has been qualified
  if (Type.hasLocalQualifiers()) {
    QualType Unqual = Type.getUnqualifiedType();
    if (Unqual->isFunctionType()) {
      diag(Loc, "function type shall not be type-qualified");
    }
  }
}

} // namespace clang::tidy::automotive

//===--- AvoidStringLiteralToNonConstCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStringLiteralToNonConstCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidStringLiteralToNonConstCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations initialized with string literals
  Finder->addMatcher(varDecl(hasInitializer(ignoringImpCasts(stringLiteral())))
                         .bind("varDecl"),
                     this);
}

void AvoidStringLiteralToNonConstCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("varDecl");
  if (!VD)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  QualType VarType = VD->getType();

  // Check if it's a pointer to char
  if (!VarType->isPointerType())
    return;

  QualType PointeeType = VarType->getPointeeType();

  // Check if pointing to char type
  if (!PointeeType->isCharType())
    return;

  // Check if pointee is const-qualified
  if (PointeeType.isConstQualified())
    return;

  diag(VD->getLocation(),
       "string literal should not be assigned to a non-const pointer");
}

} // namespace clang::tidy::automotive

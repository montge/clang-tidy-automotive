//===--- AvoidSizeofArrayParameterCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSizeofArrayParameterCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidSizeofArrayParameterCheck::registerMatchers(MatchFinder *Finder) {
  // Match sizeof expressions where the argument is a DeclRefExpr to a
  // ParmVarDecl. We need to handle implicit casts since array parameters
  // decay to pointers.
  Finder->addMatcher(
      unaryExprOrTypeTraitExpr(
          ofKind(UETT_SizeOf),
          has(ignoringParenImpCasts(
              declRefExpr(to(parmVarDecl().bind("param"))).bind("ref"))))
          .bind("sizeof"),
      this);
}

void AvoidSizeofArrayParameterCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *SizeofExpr =
      Result.Nodes.getNodeAs<UnaryExprOrTypeTraitExpr>("sizeof");
  const auto *Param = Result.Nodes.getNodeAs<ParmVarDecl>("param");

  if (!SizeofExpr || !Param)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(SizeofExpr->getBeginLoc()))
    return;

  // Check if sizeof operates on a type (not an expression)
  if (SizeofExpr->isArgumentType())
    return;

  // Get the original type as written in the declaration
  // When a parameter is declared as int arr[10], it's adjusted to int* but
  // we can check the original type info
  TypeSourceInfo *TSI = Param->getTypeSourceInfo();
  if (!TSI)
    return;

  // Get the type as written
  QualType OriginalType = TSI->getType();

  // Check if the original declaration used array syntax
  // This includes both complete arrays (int arr[10]) and incomplete arrays
  // (int arr[])
  if (!OriginalType->isArrayType())
    return;

  // The parameter was declared with array syntax but decayed to pointer
  diag(SizeofExpr->getBeginLoc(),
       "sizeof applied to parameter '%0' declared as array; this gives the "
       "size of a pointer, not the array")
      << Param->getName();

  // Add a note pointing to the parameter declaration
  diag(Param->getLocation(), "parameter declared here with array type",
       DiagnosticIDs::Note);
}

} // namespace clang::tidy::automotive

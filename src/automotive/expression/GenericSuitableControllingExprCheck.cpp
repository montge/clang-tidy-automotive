//===--- GenericSuitableControllingExprCheck.cpp - clang-tidy ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericSuitableControllingExprCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericSuitableControllingExprCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("genericSelection"), this);
}

void GenericSuitableControllingExprCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  const Expr *ControllingExpr = GenericSelection->getControllingExpr();
  if (!ControllingExpr)
    return;

  QualType ControllingType = ControllingExpr->getType();
  SourceLocation Loc = ControllingExpr->getBeginLoc();

  // Check for void type
  if (ControllingType->isVoidType()) {
    diag(Loc, "controlling expression has void type; use an expression with a "
              "complete object type");
    return;
  }

  // Check for function type (not pointer-to-function)
  if (ControllingType->isFunctionType()) {
    diag(Loc, "controlling expression has function type %0; use "
              "pointer-to-function type instead")
        << ControllingType;
    return;
  }

  // Check for incomplete types
  if (ControllingType->isIncompleteType()) {
    diag(Loc, "controlling expression has incomplete type %0; use a complete "
              "type")
        << ControllingType;
    return;
  }

  // Check for variably modified types (VLAs)
  if (ControllingType->isVariablyModifiedType()) {
    diag(Loc, "controlling expression has variably modified type %0; use a "
              "type with constant size")
        << ControllingType;
    return;
  }
}

} // namespace clang::tidy::automotive

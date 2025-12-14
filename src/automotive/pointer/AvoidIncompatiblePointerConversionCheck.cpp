//===--- AvoidIncompatiblePointerConversionCheck.cpp - clang-tidy ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidIncompatiblePointerConversionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidIncompatiblePointerConversionCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match C-style casts involving pointers
  Finder->addMatcher(cStyleCastExpr().bind("cast"), this);
}

void AvoidIncompatiblePointerConversionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<CStyleCastExpr>("cast");
  if (!Cast)
    return;

  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  QualType FromType = Cast->getSubExpr()->getType();
  QualType ToType = Cast->getType();

  // Rule 11.5: void* to object pointer
  if (FromType->isVoidPointerType() && ToType->isPointerType() &&
      !ToType->isVoidPointerType()) {
    diag(Cast->getBeginLoc(),
         "conversion from void pointer to object pointer; consider using "
         "explicit type from the start");
    return;
  }

  // Rule 11.6: pointer to void and arithmetic type conversions
  if (FromType->isVoidPointerType() && ToType->isArithmeticType()) {
    diag(Cast->getBeginLoc(),
         "conversion from void pointer to arithmetic type is not allowed");
    return;
  }
  if (FromType->isArithmeticType() && ToType->isVoidPointerType()) {
    diag(Cast->getBeginLoc(),
         "conversion from arithmetic type to void pointer is not allowed");
    return;
  }

  // Rule 11.7: pointer to object and non-integer arithmetic
  if (FromType->isPointerType() && ToType->isFloatingType()) {
    diag(Cast->getBeginLoc(),
         "conversion from pointer to floating type is not allowed");
    return;
  }
  if (FromType->isFloatingType() && ToType->isPointerType()) {
    diag(Cast->getBeginLoc(),
         "conversion from floating type to pointer is not allowed");
    return;
  }

  // Rule 11.2: conversion involving incomplete type pointer
  if (FromType->isPointerType()) {
    QualType PointeeType = FromType->getPointeeType();
    if (PointeeType->isIncompleteType() && !PointeeType->isVoidType()) {
      diag(Cast->getBeginLoc(),
           "conversion from pointer to incomplete type is not allowed");
    }
  }
}

} // namespace clang::tidy::automotive

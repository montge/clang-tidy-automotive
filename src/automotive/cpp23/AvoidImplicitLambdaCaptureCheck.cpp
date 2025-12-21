//===--- AvoidImplicitLambdaCaptureCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidImplicitLambdaCaptureCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidImplicitLambdaCaptureCheck::registerMatchers(MatchFinder *Finder) {
  // Match lambda expressions
  Finder->addMatcher(lambdaExpr().bind("lambda"), this);
}

void AvoidImplicitLambdaCaptureCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Lambda = Result.Nodes.getNodeAs<LambdaExpr>("lambda");
  if (!Lambda)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Lambda->getBeginLoc()))
    return;

  // Check the capture default
  LambdaCaptureDefault CaptureDefault = Lambda->getCaptureDefault();

  if (CaptureDefault == LCD_ByCopy) {
    diag(Lambda->getCaptureDefaultLoc(),
         "lambda uses implicit capture-by-copy [=]; all captures should be "
         "explicitly listed");
  } else if (CaptureDefault == LCD_ByRef) {
    diag(Lambda->getCaptureDefaultLoc(),
         "lambda uses implicit capture-by-reference [&]; all captures should "
         "be explicitly listed");
  }
}

} // namespace clang::tidy::automotive

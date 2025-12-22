//===--- ExplicitLambdaCaptureCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExplicitLambdaCaptureCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void ExplicitLambdaCaptureCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(lambdaExpr().bind("lambda"), this);
}

void ExplicitLambdaCaptureCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Lambda = Result.Nodes.getNodeAs<LambdaExpr>("lambda");
  if (!Lambda)
    return;

  LambdaCaptureDefault CaptureDefault = Lambda->getCaptureDefault();

  if (CaptureDefault == LCD_ByCopy) {
    diag(Lambda->getCaptureDefaultLoc(),
         "lambda uses default capture by copy '[=]'; explicitly list all "
         "captured variables instead");
  } else if (CaptureDefault == LCD_ByRef) {
    diag(Lambda->getCaptureDefaultLoc(),
         "lambda uses default capture by reference '[&]'; explicitly list all "
         "captured variables instead");
  }
}

} // namespace clang::tidy::automotive::cpp23

//===--- AvoidLambdaThisCaptureCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidLambdaThisCaptureCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidLambdaThisCaptureCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(lambdaExpr().bind("lambda"), this);
}

void AvoidLambdaThisCaptureCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Lambda = Result.Nodes.getNodeAs<LambdaExpr>("lambda");
  if (!Lambda)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Lambda->getBeginLoc()))
    return;

  LambdaCaptureDefault CaptureDefault = Lambda->getCaptureDefault();

  // Only check if there is a default capture ([=] or [&])
  if (CaptureDefault == LCD_None)
    return;

  // Check if 'this' is implicitly captured
  bool capturesThis = false;
  bool explicitThisCapture = false;

  // Iterate through all captures to check for explicit 'this' capture
  for (const LambdaCapture &Capture : Lambda->captures()) {
    if (Capture.capturesThis()) {
      capturesThis = true;
      // Check if this is an explicit capture
      if (!Capture.isImplicit()) {
        explicitThisCapture = true;
        break;
      }
    }
  }

  // If 'this' is captured and it's not explicit, emit diagnostic
  if (capturesThis && !explicitThisCapture) {
    if (CaptureDefault == LCD_ByCopy) {
      diag(Lambda->getCaptureDefaultLoc(),
           "lambda implicitly captures 'this' via [=]; use explicit [this] or "
           "[*this] capture instead");
    } else if (CaptureDefault == LCD_ByRef) {
      diag(Lambda->getCaptureDefaultLoc(),
           "lambda implicitly captures 'this' via [&]; use explicit [this] "
           "capture instead");
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

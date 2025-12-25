//===--- LocaleConstPointerCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LocaleConstPointerCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void LocaleConstPointerCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to localeconv, getenv, setlocale, strerror
  auto LocaleFunctions =
      callExpr(callee(functionDecl(hasAnyName("localeconv", "getenv",
                                              "setlocale", "strerror"))))
          .bind("call");

  // Match when the result is assigned to a non-const pointer variable
  Finder->addMatcher(
      varDecl(hasInitializer(ignoringImpCasts(LocaleFunctions)),
              hasType(pointerType(unless(pointee(isConstQualified())))))
          .bind("var"),
      this);

  // Match when the result is used in a cast to non-const
  Finder->addMatcher(
      castExpr(hasSourceExpression(ignoringParenImpCasts(LocaleFunctions)),
               hasType(pointerType(unless(pointee(isConstQualified())))))
          .bind("cast"),
      this);

  // Match when the result is assigned to an existing non-const pointer
  Finder->addMatcher(binaryOperator(hasOperatorName("="),
                                    hasRHS(ignoringImpCasts(LocaleFunctions)),
                                    hasLHS(expr(hasType(pointerType(
                                        unless(pointee(isConstQualified())))))))
                         .bind("assign"),
                     this);
}

void LocaleConstPointerCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  if (!Call)
    return;

  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  const auto *FD = Call->getDirectCallee();
  if (!FD)
    return;

  StringRef FuncName = FD->getName();

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    diag(VD->getLocation(),
         "pointer returned by '%0' shall only be used as pointer to "
         "const-qualified type; variable '%1' is not const-qualified")
        << FuncName << VD->getName();
    return;
  }

  if (const auto *Cast = Result.Nodes.getNodeAs<CastExpr>("cast")) {
    diag(Cast->getBeginLoc(),
         "pointer returned by '%0' shall only be used as pointer to "
         "const-qualified type; cast removes const qualification")
        << FuncName;
    return;
  }

  if (const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assign")) {
    diag(Assign->getOperatorLoc(),
         "pointer returned by '%0' shall only be used as pointer to "
         "const-qualified type; assignment target is not const-qualified")
        << FuncName;
  }
}

} // namespace clang::tidy::automotive::cpp23

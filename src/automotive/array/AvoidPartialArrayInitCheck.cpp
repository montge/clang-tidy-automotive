//===--- AvoidPartialArrayInitCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPartialArrayInitCheck.h"
#include "../utils/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPartialArrayInitCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      varDecl(
          hasType(arrayType()),
          hasInitializer(initListExpr(unless(automotive::isZeroInitializer()),
                                      unless(automotive::isStringLiteralInit()))
                             .bind("init")))
          .bind("arrayVar"),
      this);
}

void AvoidPartialArrayInitCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("arrayVar");
  const auto *Init = Result.Nodes.getNodeAs<InitListExpr>("init");

  if (!Var || !Init) {
    return;
  }

  const auto *ArrayTy = Result.Context->getAsConstantArrayType(Var->getType());
  if (!ArrayTy) {
    return;
  }

  uint64_t ArraySize = ArrayTy->getSize().getZExtValue();

  // Use syntactic form to get actual written initializers
  // (semantic form may include implicit value inits for gaps)
  const InitListExpr *SyntacticInit =
      Init->isSyntacticForm() ? Init : Init->getSyntacticForm();
  if (!SyntacticInit)
    SyntacticInit = Init;

  unsigned InitCount = SyntacticInit->getNumInits();

  // Skip empty initializer {} - this is explicit zero initialization
  if (InitCount == 0) {
    return;
  }

  if (InitCount < ArraySize) {
    diag(Var->getLocation(), "avoid partially initialized array");
  }
}

} // namespace clang::tidy::automotive

//===--- PartialArrayInitCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PartialArrayInitCheck.h"
#include "../utils/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void PartialArrayInitCheck::registerMatchers(MatchFinder *Finder) {
  // Match array variable declarations with initializers
  // Skip empty initializers {} (explicit zero-init)
  // Skip string literal initializations
  // Use hasCanonicalType to handle typedefs
  Finder->addMatcher(
      varDecl(
          hasType(hasCanonicalType(arrayType())),
          hasInitializer(initListExpr(unless(automotive::isZeroInitializer()),
                                      unless(automotive::isStringLiteralInit()))
                             .bind("init")))
          .bind("arrayVar"),
      this);
}

void PartialArrayInitCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("arrayVar");
  const auto *Init = Result.Nodes.getNodeAs<InitListExpr>("init");

  if (!Var || !Init) {
    return;
  }

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Var->getLocation())) {
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

  // Check if array is partially initialized
  if (InitCount < ArraySize) {
    diag(Var->getLocation(),
         "array has %0 elements but only %1 initializers provided; all "
         "elements should be explicitly initialized")
        << static_cast<unsigned>(ArraySize) << InitCount;
  }
}

} // namespace clang::tidy::automotive

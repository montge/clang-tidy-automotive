//===--- AvoidFloatingPointEqualityCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidFloatingPointEqualityCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidFloatingPointEqualityCheck::registerMatchers(MatchFinder *Finder) {
  // Match binary operators == or != where either operand is floating-point
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("=="), hasOperatorName("!=")),
                     anyOf(hasLHS(hasType(realFloatingPointType())),
                           hasRHS(hasType(realFloatingPointType()))))
          .bind("comparison"),
      this);
}

void AvoidFloatingPointEqualityCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Comparison = Result.Nodes.getNodeAs<BinaryOperator>("comparison");
  if (!Comparison)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Comparison->getOperatorLoc()))
    return;

  StringRef OpStr = Comparison->getOpcodeStr();
  diag(Comparison->getOperatorLoc(),
       "floating-point %select{equality|inequality}0 comparison using '%1'; "
       "consider using an epsilon-based comparison instead")
      << (Comparison->getOpcode() == BO_EQ ? 0 : 1) << OpStr;
}

} // namespace clang::tidy::automotive

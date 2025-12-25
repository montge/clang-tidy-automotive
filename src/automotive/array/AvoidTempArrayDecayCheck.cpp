//===--- AvoidTempArrayDecayCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTempArrayDecayCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidTempArrayDecayCheck::registerMatchers(MatchFinder *Finder) {
  // Match implicit casts from array to pointer where the source is a compound
  // literal with array type. This detects cases like:
  // - int *p = (int[]){1, 2, 3};
  // - passing compound literal arrays to pointer parameters
  // - assigning compound literal arrays to pointer variables
  //
  // The matcher finds ImplicitCastExpr nodes that:
  // 1. Have cast kind ArrayToPointerDecay
  // 2. Have a CompoundLiteralExpr as their child
  // 3. The compound literal has an array type
  Finder->addMatcher(
      implicitCastExpr(
          hasCastKind(CK_ArrayToPointerDecay),
          has(compoundLiteralExpr(hasType(arrayType())).bind("compoundLit")))
          .bind("arrayDecay"),
      this);
}

void AvoidTempArrayDecayCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ArrayDecay =
      Result.Nodes.getNodeAs<ImplicitCastExpr>("arrayDecay");
  const auto *CompoundLit =
      Result.Nodes.getNodeAs<CompoundLiteralExpr>("compoundLit");

  if (!ArrayDecay || !CompoundLit) {
    return;
  }

  // File-scope compound literals have static storage duration, not temporary
  // lifetime. Only flag compound literals at function/block scope.
  if (CompoundLit->isFileScope()) {
    return;
  }

  diag(CompoundLit->getBeginLoc(),
       "object with temporary lifetime shall not undergo array-to-pointer "
       "conversion");
}

} // namespace clang::tidy::automotive

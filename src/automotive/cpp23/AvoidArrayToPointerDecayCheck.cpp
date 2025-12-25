//===--- AvoidArrayToPointerDecayCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidArrayToPointerDecayCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidArrayToPointerDecayCheck::registerMatchers(MatchFinder *Finder) {
  // Match implicit array-to-pointer decay casts
  // This is the fundamental operation we want to detect
  Finder->addMatcher(
      implicitCastExpr(hasCastKind(CK_ArrayToPointerDecay),
                       hasSourceExpression(expr().bind("array-expr")))
          .bind("decay"),
      this);
}

void AvoidArrayToPointerDecayCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Decay = Result.Nodes.getNodeAs<ImplicitCastExpr>("decay");
  const auto *ArrayExpr = Result.Nodes.getNodeAs<Expr>("array-expr");

  if (!Decay || !ArrayExpr)
    return;

  if (Result.SourceManager->isInSystemHeader(Decay->getBeginLoc()))
    return;

  // Get the parent to understand context
  auto Parents = Result.Context->getParents(*Decay);
  if (Parents.empty())
    return;

  // Allow array subscript operations (array[i] is OK)
  if (Parents[0].get<ArraySubscriptExpr>())
    return;

  // Allow in sizeof expressions
  if (Parents[0].get<UnaryExprOrTypeTraitExpr>())
    return;

  // Allow in decltype expressions
  if (Parents[0].get<DecltypeType>())
    return;

  // Allow when used with & operator (taking address of first element)
  if (const auto *UO = Parents[0].get<UnaryOperator>()) {
    if (UO->getOpcode() == UO_AddrOf)
      return;
  }

  // Allow in string literal initializations (const char* s = "hello" is common)
  if (isa<StringLiteral>(ArrayExpr->IgnoreParenImpCasts()))
    return;

  // Get the source expression type for the diagnostic
  QualType ArrayType = ArrayExpr->getType();

  diag(Decay->getBeginLoc(),
       "array '%0' decays to pointer; explicit conversion or use of "
       "std::array/std::span recommended")
      << ArrayType.getAsString();
}

} // namespace clang::tidy::automotive::cpp23

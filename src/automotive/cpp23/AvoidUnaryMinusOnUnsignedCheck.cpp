//===--- AvoidUnaryMinusOnUnsignedCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnaryMinusOnUnsignedCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidUnaryMinusOnUnsignedCheck::registerMatchers(MatchFinder *Finder) {
  // Match unary minus operator
  Finder->addMatcher(unaryOperator(hasOperatorName("-")).bind("unary_minus"),
                     this);
}

void AvoidUnaryMinusOnUnsignedCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *UnaryOp = Result.Nodes.getNodeAs<UnaryOperator>("unary_minus");
  if (!UnaryOp)
    return;

  // Skip if in system header
  if (Result.Context->getSourceManager().isInSystemHeader(
          UnaryOp->getOperatorLoc()))
    return;

  // Get the operand, ignoring parentheses and implicit casts
  const Expr *SubExpr = UnaryOp->getSubExpr()->IgnoreParenImpCasts();
  QualType SubExprType = SubExpr->getType();

  // Check if the operand has unsigned integer type
  if (isUnsignedIntegerType(SubExprType)) {
    diag(UnaryOp->getOperatorLoc(),
         "unary minus operator shall not be applied to an expression whose "
         "underlying type is unsigned; consider using a signed type or "
         "explicit cast");
  }
}

bool AvoidUnaryMinusOnUnsignedCheck::isUnsignedIntegerType(
    QualType Type) const {
  // Remove references and get the canonical type
  Type = Type.getNonReferenceType().getCanonicalType();

  // Ignore const/volatile qualifiers
  Type = Type.getUnqualifiedType();

  // Check if it's an unsigned integer type (but not an enum)
  // This includes: unsigned char, unsigned short, unsigned int, unsigned long,
  // unsigned long long, and their fixed-width equivalents (uint8_t, etc.)
  if (Type->isUnsignedIntegerType() && !Type->isEnumeralType()) {
    return true;
  }

  return false;
}

} // namespace clang::tidy::automotive

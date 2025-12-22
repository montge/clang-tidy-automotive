//===--- AvoidEssentialTypeMismatchCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidEssentialTypeMismatchCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidEssentialTypeMismatchCheck::registerMatchers(MatchFinder *Finder) {
  // Match binary operators where usual arithmetic conversions apply
  Finder->addMatcher(
      binaryOperator(
          anyOf(hasAnyOperatorName("+", "-", "*", "/", "%"),
                hasAnyOperatorName("<", ">", "<=", ">=", "==", "!=")))
          .bind("binop"),
      this);
}

void AvoidEssentialTypeMismatchCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop");
  if (!BinOp)
    return;

  if (Result.SourceManager->isInSystemHeader(BinOp->getOperatorLoc()))
    return;

  // Skip if in macro
  if (BinOp->getOperatorLoc().isMacroID())
    return;

  const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
  const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

  EssentialType LHSET = getEssentialType(LHS->getType());
  EssentialType RHSET = getEssentialType(RHS->getType());

  // Skip if both are "Other" or same type
  if (LHSET == RHSET)
    return;
  if (LHSET == EssentialType::Other || RHSET == EssentialType::Other)
    return;

  // MISRA C:2012 Rule 10.4 - all essential type mismatches should be flagged
  // The following combinations are problematic:
  // - Signed vs Unsigned
  // - Integer vs Floating
  // - Character vs Integer
  // - Boolean vs anything non-boolean
  // - Enum vs anything non-enum

  bool isMismatch = false;

  // Signed/unsigned mismatch
  if ((LHSET == EssentialType::SignedInt &&
       RHSET == EssentialType::UnsignedInt) ||
      (LHSET == EssentialType::UnsignedInt &&
       RHSET == EssentialType::SignedInt)) {
    isMismatch = true;
  }

  // Integer/floating mismatch
  if ((LHSET == EssentialType::FloatingPoint &&
       (RHSET == EssentialType::SignedInt ||
        RHSET == EssentialType::UnsignedInt)) ||
      (RHSET == EssentialType::FloatingPoint &&
       (LHSET == EssentialType::SignedInt ||
        LHSET == EssentialType::UnsignedInt))) {
    isMismatch = true;
  }

  // Character vs numeric type mismatch
  if ((LHSET == EssentialType::Character &&
       (RHSET == EssentialType::SignedInt ||
        RHSET == EssentialType::UnsignedInt ||
        RHSET == EssentialType::FloatingPoint)) ||
      (RHSET == EssentialType::Character &&
       (LHSET == EssentialType::SignedInt ||
        LHSET == EssentialType::UnsignedInt ||
        LHSET == EssentialType::FloatingPoint))) {
    isMismatch = true;
  }

  // Boolean vs non-boolean (except logical operators which expect booleans)
  if (!BinOp->isLogicalOp()) {
    if ((LHSET == EssentialType::Boolean && RHSET != EssentialType::Boolean) ||
        (RHSET == EssentialType::Boolean && LHSET != EssentialType::Boolean)) {
      isMismatch = true;
    }
  }

  // Enum vs non-enum (except same enum or comparison with constant)
  if ((LHSET == EssentialType::Enum && RHSET != EssentialType::Enum) ||
      (RHSET == EssentialType::Enum && LHSET != EssentialType::Enum)) {
    isMismatch = true;
  }

  if (isMismatch) {
    diag(BinOp->getOperatorLoc(),
         "operands have different essential type categories: '%0' and '%1'")
        << getEssentialTypeName(LHSET) << getEssentialTypeName(RHSET);
  }
}

AvoidEssentialTypeMismatchCheck::EssentialType
AvoidEssentialTypeMismatchCheck::getEssentialType(QualType Type) const {
  Type = Type.getCanonicalType().getUnqualifiedType();

  if (Type->isBooleanType())
    return EssentialType::Boolean;

  if (Type->isAnyCharacterType())
    return EssentialType::Character;

  if (Type->isEnumeralType())
    return EssentialType::Enum;

  if (Type->isFloatingType())
    return EssentialType::FloatingPoint;

  if (Type->isIntegerType()) {
    if (Type->isUnsignedIntegerType())
      return EssentialType::UnsignedInt;
    if (Type->isSignedIntegerType())
      return EssentialType::SignedInt;
  }

  return EssentialType::Other;
}

StringRef
AvoidEssentialTypeMismatchCheck::getEssentialTypeName(EssentialType ET) const {
  switch (ET) {
  case EssentialType::Boolean:
    return "Boolean";
  case EssentialType::Character:
    return "Character";
  case EssentialType::SignedInt:
    return "Signed";
  case EssentialType::UnsignedInt:
    return "Unsigned";
  case EssentialType::FloatingPoint:
    return "Floating";
  case EssentialType::Enum:
    return "Enum";
  case EssentialType::Other:
    return "Other";
  }
  return "Unknown";
}

} // namespace clang::tidy::automotive

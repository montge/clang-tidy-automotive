//===--- AvoidNarrowingCompoundAssignmentCheck.cpp - clang-tidy -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNarrowingCompoundAssignmentCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidNarrowingCompoundAssignmentCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match all assignment expressions
  Finder->addMatcher(binaryOperator(isAssignmentOperator()).bind("assign"),
                     this);
}

void AvoidNarrowingCompoundAssignmentCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assign");
  if (!Assign)
    return;

  if (Result.SourceManager->isInSystemHeader(Assign->getOperatorLoc()))
    return;

  QualType LHSType = Assign->getLHS()->getType();
  QualType RHSType = Assign->getRHS()->IgnoreParenImpCasts()->getType();

  EssentialType LHSET = getEssentialType(LHSType);
  EssentialType RHSET = getEssentialType(RHSType);

  // Check for different essential type categories
  if (isDifferentCategory(RHSET, LHSET)) {
    diag(Assign->getOperatorLoc(),
         "expression of essentially '%0' type assigned to object of "
         "essentially '%1' type")
        << (RHSET == EssentialType::SignedInt       ? "signed"
            : RHSET == EssentialType::UnsignedInt   ? "unsigned"
            : RHSET == EssentialType::FloatingPoint ? "floating"
                                                    : "different category")
        << (LHSET == EssentialType::SignedInt       ? "signed"
            : LHSET == EssentialType::UnsignedInt   ? "unsigned"
            : LHSET == EssentialType::FloatingPoint ? "floating"
                                                    : "different category");
    return;
  }

  // Check for narrowing within same category
  if (isNarrowing(RHSType, LHSType, *Result.Context)) {
    diag(Assign->getOperatorLoc(),
         "assignment results in narrowing conversion from wider to narrower "
         "type");
  }
}

AvoidNarrowingCompoundAssignmentCheck::EssentialType
AvoidNarrowingCompoundAssignmentCheck::getEssentialType(QualType Type) const {
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

unsigned AvoidNarrowingCompoundAssignmentCheck::getTypeWidth(
    QualType Type, const ASTContext &Ctx) const {
  Type = Type.getCanonicalType();
  if (Type->isIntegerType() || Type->isFloatingType())
    return Ctx.getTypeSize(Type);
  return 0;
}

bool AvoidNarrowingCompoundAssignmentCheck::isNarrowing(
    QualType From, QualType To, const ASTContext &Ctx) const {
  unsigned FromWidth = getTypeWidth(From, Ctx);
  unsigned ToWidth = getTypeWidth(To, Ctx);

  if (FromWidth == 0 || ToWidth == 0)
    return false;

  return FromWidth > ToWidth;
}

bool AvoidNarrowingCompoundAssignmentCheck::isDifferentCategory(
    EssentialType From, EssentialType To) const {
  if (From == To)
    return false;

  // Allow signed/unsigned mixing to be caught by narrowing instead
  if ((From == EssentialType::SignedInt && To == EssentialType::UnsignedInt) ||
      (From == EssentialType::UnsignedInt && To == EssentialType::SignedInt))
    return true;

  // Other category mismatches
  if (From == EssentialType::FloatingPoint &&
      To != EssentialType::FloatingPoint)
    return true;
  if (To == EssentialType::FloatingPoint &&
      From != EssentialType::FloatingPoint)
    return true;

  return false;
}

} // namespace clang::tidy::automotive

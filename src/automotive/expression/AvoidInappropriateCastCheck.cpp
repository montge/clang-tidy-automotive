//===--- AvoidInappropriateCastCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidInappropriateCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidInappropriateCastCheck::registerMatchers(MatchFinder *Finder) {
  // Match C-style casts
  Finder->addMatcher(cStyleCastExpr().bind("cast"), this);
}

void AvoidInappropriateCastCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<CStyleCastExpr>("cast");
  if (!Cast)
    return;

  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  QualType FromType = Cast->getSubExpr()->getType();
  QualType ToType = Cast->getType();

  EssentialType FromET = getEssentialType(FromType);
  EssentialType ToET = getEssentialType(ToType);

  if (isInappropriateCast(FromET, ToET)) {
    diag(Cast->getBeginLoc(), "cast from essentially '%0' type to essentially "
                              "'%1' type may indicate design issues")
        << (FromET == EssentialType::SignedInt       ? "signed"
            : FromET == EssentialType::UnsignedInt   ? "unsigned"
            : FromET == EssentialType::Boolean       ? "boolean"
            : FromET == EssentialType::Character     ? "character"
            : FromET == EssentialType::FloatingPoint ? "floating"
            : FromET == EssentialType::Enum          ? "enum"
                                                     : "other")
        << (ToET == EssentialType::SignedInt       ? "signed"
            : ToET == EssentialType::UnsignedInt   ? "unsigned"
            : ToET == EssentialType::Boolean       ? "boolean"
            : ToET == EssentialType::Character     ? "character"
            : ToET == EssentialType::FloatingPoint ? "floating"
            : ToET == EssentialType::Enum          ? "enum"
                                                   : "other");
  }
}

AvoidInappropriateCastCheck::EssentialType
AvoidInappropriateCastCheck::getEssentialType(QualType Type) const {
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

bool AvoidInappropriateCastCheck::isInappropriateCast(EssentialType From,
                                                      EssentialType To) const {
  // Cast to boolean is inappropriate unless from boolean
  if (To == EssentialType::Boolean && From != EssentialType::Boolean)
    return true;

  // Cast from boolean to non-boolean is inappropriate
  if (From == EssentialType::Boolean && To != EssentialType::Boolean)
    return true;

  // Cast to character from non-character (except unsigned)
  if (To == EssentialType::Character && From != EssentialType::Character &&
      From != EssentialType::UnsignedInt)
    return true;

  // Cast to enum is inappropriate
  if (To == EssentialType::Enum && From != EssentialType::Enum)
    return true;

  return false;
}

} // namespace clang::tidy::automotive

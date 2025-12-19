//===--- AvoidNarrowingConversionCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNarrowingConversionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidNarrowingConversionCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match implicit casts that could be narrowing
  Finder->addMatcher(
      implicitCastExpr(hasImplicitDestinationType(qualType().bind("toType")),
                       has(expr(hasType(qualType().bind("fromType")))))
          .bind("cast"),
      this);
}

void AvoidNarrowingConversionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<ImplicitCastExpr>("cast");
  if (!Cast)
    return;

  // Skip casts in system headers
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  // Skip casts in macros
  if (Cast->getBeginLoc().isMacroID())
    return;

  // Only check certain cast kinds that could be narrowing
  auto CastKind = Cast->getCastKind();
  if (CastKind != CK_IntegralCast && CastKind != CK_FloatingToIntegral &&
      CastKind != CK_FloatingCast && CastKind != CK_IntegralToFloating)
    return;

  QualType FromType = Cast->getSubExpr()->getType().getCanonicalType();
  QualType ToType = Cast->getType().getCanonicalType();

  if (!isNarrowingConversion(FromType, ToType, *Result.Context))
    return;

  diag(Cast->getBeginLoc(),
       "narrowing conversion from %0 to %1 may result in data loss")
      << FromType << ToType;
}

bool AvoidNarrowingConversionCheck::isNarrowingConversion(
    QualType FromType, QualType ToType, const ASTContext &Ctx) const {
  // Get canonical types
  FromType = FromType.getCanonicalType();
  ToType = ToType.getCanonicalType();

  // Same type is not narrowing
  if (FromType == ToType)
    return false;

  const auto *FromBuiltin = FromType->getAs<BuiltinType>();
  const auto *ToBuiltin = ToType->getAs<BuiltinType>();

  if (!FromBuiltin || !ToBuiltin)
    return false;

  // Floating-point to integer is always narrowing
  if (FromType->isFloatingType() && ToType->isIntegerType())
    return true;

  // Larger floating-point to smaller floating-point
  if (FromType->isFloatingType() && ToType->isFloatingType()) {
    unsigned FromSize = Ctx.getTypeSize(FromType);
    unsigned ToSize = Ctx.getTypeSize(ToType);
    return FromSize > ToSize;
  }

  // Integer conversions
  if (FromType->isIntegerType() && ToType->isIntegerType()) {
    unsigned FromSize = Ctx.getTypeSize(FromType);
    unsigned ToSize = Ctx.getTypeSize(ToType);

    // Larger to smaller is narrowing
    if (FromSize > ToSize)
      return true;

    // Same size but signed to unsigned or vice versa with potential loss
    if (FromSize == ToSize &&
        FromType->isSignedIntegerType() != ToType->isSignedIntegerType())
      return true;
  }

  // Integer to floating-point can lose precision for large integers
  if (FromType->isIntegerType() && ToType->isFloatingType()) {
    unsigned FromSize = Ctx.getTypeSize(FromType);
    // int64_t to float loses precision (float has ~24 bits of mantissa)
    // int64_t to double can also lose precision (double has ~53 bits)
    if (ToType->isSpecificBuiltinType(BuiltinType::Float) && FromSize > 24)
      return true;
    if (ToType->isSpecificBuiltinType(BuiltinType::Double) && FromSize > 53)
      return true;
  }

  return false;
}

} // namespace clang::tidy::automotive

//===--- AvoidImplicitConversionCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidImplicitConversionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidImplicitConversionCheck::registerMatchers(MatchFinder *Finder) {
  // Match implicit casts that are not trivial conversions
  // This catches conversions in function arguments, return statements,
  // and variable initializations
  Finder->addMatcher(
      implicitCastExpr(unless(isExpansionInSystemHeader()),
                       unless(isInTemplateInstantiation()),
                       // Skip implicit casts inside explicit casts
                       unless(hasAncestor(explicitCastExpr())),
                       // Only match casts that change the type meaningfully
                       hasImplicitDestinationType(
                           qualType(unless(isAnyPointer())).bind("toType")),
                       hasSourceExpression(expr().bind("fromExpr")))
          .bind("cast"),
      this);
}

bool AvoidImplicitConversionCheck::isSignificantConversion(
    QualType From, QualType To, const ASTContext &Ctx) const {
  // Skip null or invalid types
  if (From.isNull() || To.isNull())
    return false;

  // Get canonical types for comparison
  From = From.getCanonicalType();
  To = To.getCanonicalType();

  // Same type - no conversion needed
  if (From == To)
    return false;

  // Skip dependent types (templates) that may not have complete type info
  if (From->isDependentType() || To->isDependentType())
    return false;

  // Skip incomplete types that may crash getTypeSize
  if (From->isIncompleteType() || To->isIncompleteType())
    return false;

  // Ignore array to pointer decay
  if (From->isArrayType() && To->isPointerType())
    return false;

  // Ignore function to pointer decay
  if (From->isFunctionType() && To->isPointerType())
    return false;

  // Ignore lvalue to rvalue conversions
  if (From.getUnqualifiedType() == To.getUnqualifiedType())
    return false;

  // Check for numeric conversions
  if (From->isArithmeticType() && To->isArithmeticType()) {
    // Integer to floating point - significant
    if (From->isIntegerType() && To->isFloatingType())
      return true;

    // Floating point to integer - significant (but covered by narrowing check)
    if (From->isFloatingType() && To->isIntegerType())
      return true;

    // Float to double promotion - less significant, allow
    // Check by type size: float (32 bit) to double (64 bit)
    if (From->isFloatingType() && To->isFloatingType()) {
      // Guard against types that may not have size info
      if (!Ctx.getTypeInfo(From).Width || !Ctx.getTypeInfo(To).Width)
        return false;
      uint64_t FromSize = Ctx.getTypeSize(From);
      uint64_t ToSize = Ctx.getTypeSize(To);
      if (FromSize < ToSize)
        return false; // Promotion is OK
    }

    // Signed/unsigned mismatch
    if (From->isSignedIntegerType() && To->isUnsignedIntegerType())
      return true;
    if (From->isUnsignedIntegerType() && To->isSignedIntegerType())
      return true;

    // Standard integer promotions (char/short to int) - allow
    // Guard against types that may not have size info
    if (!Ctx.getTypeInfo(From).Width || !Ctx.getTypeInfo(To).Width)
      return false;
    uint64_t FromSize = Ctx.getTypeSize(From);
    uint64_t ToSize = Ctx.getTypeSize(To);
    if (FromSize < 32 && ToSize >= 32 && From->isIntegerType() &&
        To->isIntegerType())
      return false;
  }

  // Bool conversions
  if (To->isBooleanType() && !From->isBooleanType())
    return true;

  // Pointer conversions (except null pointer)
  if (From->isPointerType() && To->isPointerType() && From != To)
    return true;

  return false;
}

void AvoidImplicitConversionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<ImplicitCastExpr>("cast");
  if (!Cast)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  // Skip if in a macro
  if (Cast->getBeginLoc().isMacroID())
    return;

  // Get the cast kind
  CastKind Kind = Cast->getCastKind();

  // Skip trivial casts
  if (Kind == CK_LValueToRValue || Kind == CK_NoOp ||
      Kind == CK_ArrayToPointerDecay || Kind == CK_FunctionToPointerDecay ||
      Kind == CK_NullToPointer || Kind == CK_NullToMemberPointer ||
      Kind == CK_ConstructorConversion || Kind == CK_UserDefinedConversion)
    return;

  QualType FromType = Cast->getSubExpr()->getType();
  QualType ToType = Cast->getType();

  if (!isSignificantConversion(FromType, ToType, *Result.Context))
    return;

  diag(Cast->getBeginLoc(),
       "implicit conversion from %0 to %1; consider using an explicit cast")
      << FromType << ToType;
}

} // namespace clang::tidy::automotive

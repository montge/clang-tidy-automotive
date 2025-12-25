//===--- AvoidMemberPointerConversionCheck.cpp - clang-tidy --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidMemberPointerConversionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidMemberPointerConversionCheck::registerMatchers(MatchFinder *Finder) {
  // Match static_cast and reinterpret_cast expressions that convert
  // member pointer types
  Finder->addMatcher(
      cxxStaticCastExpr(hasDestinationType(qualType(hasCanonicalType(
                            memberPointerType().bind("toType")))),
                        hasSourceExpression(hasType(qualType(hasCanonicalType(
                            memberPointerType().bind("fromType"))))))
          .bind("staticCast"),
      this);

  Finder->addMatcher(cxxReinterpretCastExpr(
                         hasDestinationType(qualType(hasCanonicalType(
                             memberPointerType().bind("toType")))),
                         hasSourceExpression(hasType(qualType(hasCanonicalType(
                             memberPointerType().bind("fromType"))))))
                         .bind("reinterpretCast"),
                     this);
}

bool AvoidMemberPointerConversionCheck::isValidMemberPointerConversion(
    const CXXRecordDecl *FromClass, const CXXRecordDecl *ToClass) const {
  if (!FromClass || !ToClass)
    return false;

  // If classes are the same, conversion is valid
  if (FromClass->getCanonicalDecl() == ToClass->getCanonicalDecl())
    return true;

  // Check if FromClass is derived from ToClass (derived to base is safe)
  if (FromClass->isDerivedFrom(ToClass))
    return true;

  // All other conversions are invalid
  return false;
}

void AvoidMemberPointerConversionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *StaticCast =
      Result.Nodes.getNodeAs<CXXStaticCastExpr>("staticCast");
  const auto *ReinterpretCast =
      Result.Nodes.getNodeAs<CXXReinterpretCastExpr>("reinterpretCast");
  const auto *ToMemberPtr = Result.Nodes.getNodeAs<MemberPointerType>("toType");
  const auto *FromMemberPtr =
      Result.Nodes.getNodeAs<MemberPointerType>("fromType");

  const CastExpr *Cast = StaticCast
                             ? static_cast<const CastExpr *>(StaticCast)
                             : static_cast<const CastExpr *>(ReinterpretCast);

  if (!Cast || !ToMemberPtr || !FromMemberPtr)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  // Get the class types from member pointer types
  const Type *ToClassType = ToMemberPtr->getClass();
  const Type *FromClassType = FromMemberPtr->getClass();

  if (!ToClassType || !FromClassType)
    return;

  const auto *ToClass = ToClassType->getAsCXXRecordDecl();
  const auto *FromClass = FromClassType->getAsCXXRecordDecl();

  if (!ToClass || !FromClass)
    return;

  // reinterpret_cast is always forbidden for member pointer conversions
  if (ReinterpretCast) {
    diag(Cast->getBeginLoc(),
         "reinterpret_cast used to convert pointer-to-member from '%0' to "
         "'%1'; invalid member pointer conversion")
        << FromClass->getQualifiedNameAsString()
        << ToClass->getQualifiedNameAsString();
    return;
  }

  // For static_cast, check if conversion is valid
  if (StaticCast && !isValidMemberPointerConversion(FromClass, ToClass)) {
    diag(Cast->getBeginLoc(),
         "static_cast used to convert pointer-to-member from '%0' to '%1'; "
         "conversion between unrelated classes is invalid")
        << FromClass->getQualifiedNameAsString()
        << ToClass->getQualifiedNameAsString();
  }
}

} // namespace clang::tidy::automotive::cpp23

//===--- AvoidPointerTypeCastCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPointerTypeCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPointerTypeCastCheck::registerMatchers(MatchFinder *Finder) {
  // Match C-style casts between pointer types
  Finder->addMatcher(cStyleCastExpr(hasSourceExpression(hasType(pointerType())),
                                    hasType(pointerType()))
                         .bind("cast"),
                     this);
}

void AvoidPointerTypeCastCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<CStyleCastExpr>("cast");
  if (!Cast)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  // Get the source and destination types
  QualType SrcType = Cast->getSubExpr()->getType();
  QualType DstType = Cast->getType();

  // Get pointee types
  const PointerType *SrcPtr = SrcType->getAs<PointerType>();
  const PointerType *DstPtr = DstType->getAs<PointerType>();

  if (!SrcPtr || !DstPtr)
    return;

  QualType SrcPointee = SrcPtr->getPointeeType();
  QualType DstPointee = DstPtr->getPointeeType();

  // Get canonical types to strip typedefs and qualifiers
  QualType SrcCanonical = SrcPointee.getCanonicalType().getUnqualifiedType();
  QualType DstCanonical = DstPointee.getCanonicalType().getUnqualifiedType();

  // Skip if same type (ignoring qualifiers and typedefs)
  if (SrcCanonical == DstCanonical)
    return;

  // Allow casts to/from void*
  if (SrcCanonical->isVoidType() || DstCanonical->isVoidType())
    return;

  // Allow casts to char* or unsigned char* (commonly used for byte access)
  if (DstCanonical->isCharType() ||
      (DstCanonical->isSpecificBuiltinType(BuiltinType::UChar)))
    return;

  // Allow casts from char* or unsigned char*
  if (SrcCanonical->isCharType() ||
      (SrcCanonical->isSpecificBuiltinType(BuiltinType::UChar)))
    return;

  // Report incompatible pointer cast
  diag(Cast->getBeginLoc(),
       "cast from %0 to %1 converts between incompatible pointer types")
      << SrcType << DstType;
}

} // namespace clang::tidy::automotive

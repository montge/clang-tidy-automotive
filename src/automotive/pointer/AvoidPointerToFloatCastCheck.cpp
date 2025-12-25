//===--- AvoidPointerToFloatCastCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPointerToFloatCastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPointerToFloatCastCheck::registerMatchers(MatchFinder *Finder) {
  // Match casts from pointer to floating-point type
  Finder->addMatcher(castExpr(hasSourceExpression(hasType(pointerType())),
                              hasType(realFloatingPointType()))
                         .bind("ptrToFloat"),
                     this);

  // Match casts from floating-point type to pointer
  Finder->addMatcher(
      castExpr(hasSourceExpression(hasType(realFloatingPointType())),
               hasType(pointerType()))
          .bind("floatToPtr"),
      this);
}

void AvoidPointerToFloatCastCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Handle pointer to floating-point cast
  if (const auto *Cast = Result.Nodes.getNodeAs<CastExpr>("ptrToFloat")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
      return;

    // Skip implicit casts (only explicit casts are violations)
    if (isa<ImplicitCastExpr>(Cast))
      return;

    // Report explicit casts
    if (isa<CStyleCastExpr>(Cast) || isa<CXXReinterpretCastExpr>(Cast) ||
        isa<CXXStaticCastExpr>(Cast) || isa<CXXFunctionalCastExpr>(Cast)) {
      diag(Cast->getBeginLoc(),
           "cast from pointer type %0 to floating-point type %1")
          << Cast->getSubExpr()->getType() << Cast->getType();
    }
    return;
  }

  // Handle floating-point to pointer cast
  if (const auto *Cast = Result.Nodes.getNodeAs<CastExpr>("floatToPtr")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
      return;

    // Skip implicit casts (only explicit casts are violations)
    if (isa<ImplicitCastExpr>(Cast))
      return;

    // Report explicit casts
    if (isa<CStyleCastExpr>(Cast) || isa<CXXReinterpretCastExpr>(Cast) ||
        isa<CXXStaticCastExpr>(Cast) || isa<CXXFunctionalCastExpr>(Cast)) {
      diag(Cast->getBeginLoc(),
           "cast from floating-point type %0 to pointer type %1")
          << Cast->getSubExpr()->getType() << Cast->getType();
    }
  }
}

} // namespace clang::tidy::automotive

//===--- AvoidOutOfBoundsAccessCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidOutOfBoundsAccessCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidOutOfBoundsAccessCheck::registerMatchers(MatchFinder *Finder) {
  // Match array subscript expressions
  Finder->addMatcher(arraySubscriptExpr().bind("subscript"), this);
}

void AvoidOutOfBoundsAccessCheck::check(
    const MatchFinder::MatchResult &Result) {

  const auto *ASE = Result.Nodes.getNodeAs<ArraySubscriptExpr>("subscript");
  if (!ASE)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(ASE->getBeginLoc()))
    return;

  checkArraySubscript(ASE, Result.Context);
}

void AvoidOutOfBoundsAccessCheck::checkArraySubscript(
    const ArraySubscriptExpr *ASE, ASTContext *Context) {

  const Expr *Base = ASE->getBase()->IgnoreParenImpCasts();
  const Expr *Index = ASE->getIdx()->IgnoreParenImpCasts();

  // Try to get the array size
  std::optional<uint64_t> ArraySize;

  QualType BaseType = Base->getType();

  // Handle array types
  if (const auto *CAT = Context->getAsConstantArrayType(BaseType)) {
    ArraySize = CAT->getSize().getZExtValue();
  }
  // Handle pointers to arrays
  else if (BaseType->isPointerType()) {
    QualType PointeeType = BaseType->getPointeeType();
    if (const auto *CAT = Context->getAsConstantArrayType(PointeeType)) {
      ArraySize = CAT->getSize().getZExtValue();
    }
  }
  // Handle array references
  else if (BaseType->isReferenceType()) {
    QualType ReferencedType = BaseType.getNonReferenceType();
    if (const auto *CAT = Context->getAsConstantArrayType(ReferencedType)) {
      ArraySize = CAT->getSize().getZExtValue();
    }
  }

  // If we couldn't determine the array size, we can't check bounds
  if (!ArraySize)
    return;

  // Try to evaluate the index as a constant
  Expr::EvalResult IndexResult;
  if (!Index->EvaluateAsRValue(IndexResult, *Context))
    return;

  if (!IndexResult.Val.isInt())
    return;

  const llvm::APSInt &IndexValue = IndexResult.Val.getInt();

  // Check for negative index
  if (IndexValue.isNegative()) {
    diag(ASE->getBeginLoc(),
         "array subscript is negative; object accessed out of bounds "
         "(MISRA C++:2023 Rule 13.1)");
    return;
  }

  // Check if index is >= array size
  uint64_t IndexVal = IndexValue.getZExtValue();
  if (IndexVal >= ArraySize.value()) {
    diag(ASE->getBeginLoc(),
         "array subscript is out of bounds; valid range is [0, %0] "
         "(MISRA C++:2023 Rule 13.1)")
        << static_cast<unsigned>(ArraySize.value() - 1);
  }
}

} // namespace clang::tidy::automotive::cpp23

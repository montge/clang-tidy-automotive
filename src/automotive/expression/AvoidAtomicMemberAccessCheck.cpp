//===--- AvoidAtomicMemberAccessCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidAtomicMemberAccessCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidAtomicMemberAccessCheck::registerMatchers(MatchFinder *Finder) {
  // Match member expressions where the base type is atomic
  Finder->addMatcher(memberExpr().bind("member"), this);
}

void AvoidAtomicMemberAccessCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *ME = Result.Nodes.getNodeAs<MemberExpr>("member");
  if (!ME)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(ME->getMemberLoc()))
    return;

  // Get the base expression, ignoring implicit casts
  const Expr *Base = ME->getBase();
  if (!Base)
    return;

  // Look through implicit casts to find the actual base type
  // Clang adds an AtomicToNonAtomic implicit cast when accessing atomic
  // struct members
  while (const auto *ICE = dyn_cast<ImplicitCastExpr>(Base)) {
    // Check if this is an AtomicToNonAtomic cast
    if (ICE->getCastKind() == CK_AtomicToNonAtomic) {
      // Found atomic member access - the sub-expression type is atomic
      QualType SubExprType = ICE->getSubExpr()->getType();

      // If pointer, get pointee type
      if (SubExprType->isPointerType()) {
        SubExprType = SubExprType->getPointeeType();
      }

      const Type *TypePtr = SubExprType.getTypePtrOrNull();
      if (!TypePtr)
        return;

      const AtomicType *AT = TypePtr->getAs<AtomicType>();
      if (!AT)
        return;

      QualType UnderlyingType = AT->getValueType();
      if (!UnderlyingType->isRecordType())
        return;

      diag(ME->getMemberLoc(),
           "direct member access on atomic struct/union breaks atomicity; "
           "use atomic_load/atomic_store on the entire object");
      return;
    }
    Base = ICE->getSubExpr();
  }

  // Also check direct pointer-to-atomic access (ptr->member)
  QualType BaseType = Base->getType();
  if (BaseType->isPointerType()) {
    QualType PointeeType = BaseType->getPointeeType();
    const Type *TypePtr = PointeeType.getTypePtrOrNull();
    if (TypePtr) {
      if (const AtomicType *AT = TypePtr->getAs<AtomicType>()) {
        if (AT->getValueType()->isRecordType()) {
          diag(ME->getMemberLoc(),
               "direct member access on atomic struct/union breaks atomicity; "
               "use atomic_load/atomic_store on the entire object");
        }
      }
    }
  }
}

} // namespace clang::tidy::automotive

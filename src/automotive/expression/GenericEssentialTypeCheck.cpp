//===--- GenericEssentialTypeCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericEssentialTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericEssentialTypeCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(genericSelectionExpr().bind("generic"), this);
}

void GenericEssentialTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Generic = Result.Nodes.getNodeAs<GenericSelectionExpr>("generic");
  if (!Generic)
    return;

  SourceLocation Loc = Generic->getGenericLoc();

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Loc))
    return;

  const Expr *ControlExpr = Generic->getControllingExpr();
  if (!ControlExpr)
    return;

  // Get the standard type (post-promotion, used by _Generic)
  QualType StandardType = ControlExpr->getType();
  EssentialType StdET = getEssentialType(StandardType);

  // Determine the essential type based on the expression kind
  const Expr *InnerExpr = ControlExpr->IgnoreParenImpCasts();
  EssentialType EssET = EssentialType::Other;

  // Case 1: Character literal - essential type is Character
  if (isa<CharacterLiteral>(InnerExpr)) {
    EssET = EssentialType::Character;
  }
  // Case 2: Enum constant reference
  else if (const auto *DRE = dyn_cast<DeclRefExpr>(InnerExpr)) {
    if (isa<EnumConstantDecl>(DRE->getDecl())) {
      EssET = EssentialType::Enum;
    } else if (DRE->getType()->isEnumeralType()) {
      EssET = EssentialType::Enum;
    } else {
      EssET = getEssentialType(DRE->getType());
    }
  }
  // Case 2b: Constant array subscript or other expression yielding enum
  // constant
  else if (InnerExpr->getType()->isEnumeralType()) {
    EssET = EssentialType::Enum;
  }
  // Case 3: Binary arithmetic - check if operands were promoted
  else if (const auto *BinOp = dyn_cast<BinaryOperator>(InnerExpr)) {
    if (BinOp->isAdditiveOp() || BinOp->isMultiplicativeOp()) {
      // Get operand types before promotion
      const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
      const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();
      QualType LHST = LHS->getType();
      QualType RHST = RHS->getType();

      // If both operands have smaller type than the result, there was promotion
      if (LHST->isIntegerType() && RHST->isIntegerType()) {
        ASTContext &Ctx = *Result.Context;
        unsigned LHSRank = Ctx.getIntWidth(LHST);
        unsigned RHSRank = Ctx.getIntWidth(RHST);
        unsigned ResultRank = Ctx.getIntWidth(StandardType);

        if (LHSRank < ResultRank && RHSRank < ResultRank) {
          // Promotion occurred - report type mismatch
          // Use a special marker to indicate promoted type
          diag(ControlExpr->getBeginLoc(),
               "controlling expression was promoted from '%0' to '%1'; "
               "_Generic will select based on '%1' not '%0'")
              << LHST.getAsString() << StandardType.getAsString();
          return;
        }
      }
      EssET = getEssentialType(StandardType);
    } else {
      EssET = getEssentialType(StandardType);
    }
  }
  // Default case
  else {
    EssET = getEssentialType(InnerExpr->getType());
  }

  // If types match, no issue
  if (StdET == EssET)
    return;

  // Exception: integer constant expressions with essentially signed/unsigned
  // type of lower rank than int (but not char constants or boolean) are
  // allowed. This does NOT apply to enum constants.
  if (isIntegerConstantExpression(ControlExpr, *Result.Context)) {
    // Character literals always violate
    if (isa<CharacterLiteral>(InnerExpr))
      ; // Fall through to report
    // Boolean types always violate
    else if (EssET == EssentialType::Boolean)
      ; // Fall through to report
    // Enum constants always violate (enum is not signed/unsigned integer)
    else if (EssET == EssentialType::Enum)
      ; // Fall through to report
    // Other integer constant expressions (like 10u, 250+350) are allowed
    else if (EssET == EssentialType::SignedInt ||
             EssET == EssentialType::UnsignedInt)
      return; // Allowed by exception
  }

  // Report the mismatch
  diag(ControlExpr->getBeginLoc(),
       "controlling expression has essential type '%0' but standard type '%1'; "
       "the standard type determines which _Generic association is selected")
      << getEssentialTypeName(EssET) << getEssentialTypeName(StdET);
}

GenericEssentialTypeCheck::EssentialType
GenericEssentialTypeCheck::getEssentialType(QualType Type) const {
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
GenericEssentialTypeCheck::getEssentialTypeName(EssentialType ET) const {
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

bool GenericEssentialTypeCheck::isIntegerConstantExpression(
    const Expr *E, ASTContext &Ctx) const {
  Expr::EvalResult Result;
  return E->EvaluateAsInt(Result, Ctx);
}

} // namespace clang::tidy::automotive

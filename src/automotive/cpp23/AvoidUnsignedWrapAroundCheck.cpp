//===--- AvoidUnsignedWrapAroundCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnsignedWrapAroundCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidUnsignedWrapAroundCheck::registerMatchers(MatchFinder *Finder) {
  // Match binary operators on unsigned integer types that may cause wrap-around
  // Focus on subtraction, addition, and multiplication
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("-"), hasOperatorName("+"),
                           hasOperatorName("*")),
                     hasType(isUnsignedInteger()))
          .bind("binop"),
      this);

  // Match compound assignment operators that may cause wrap-around
  // Use binaryOperator since CompoundAssignOperator is a subclass
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("-="), hasOperatorName("+="),
                           hasOperatorName("*=")))
          .bind("compound"),
      this);
}

void AvoidUnsignedWrapAroundCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binop");
  const auto *CompoundOp =
      Result.Nodes.getNodeAs<CompoundAssignOperator>("compound");

  if (BinOp) {
    checkBinaryOperator(BinOp, Result.Context);
  } else if (CompoundOp) {
    checkCompoundAssignment(CompoundOp, Result.Context);
  }
}

bool AvoidUnsignedWrapAroundCheck::isUnsignedIntegerType(QualType Type) const {
  // Remove references and get the canonical type
  Type = Type.getNonReferenceType().getCanonicalType();

  // Ignore const/volatile qualifiers
  Type = Type.getUnqualifiedType();

  // Check if it's an unsigned integer type (but not an enum or bool)
  if (Type->isUnsignedIntegerType() && !Type->isEnumeralType() &&
      !Type->isBooleanType()) {
    return true;
  }

  return false;
}

bool AvoidUnsignedWrapAroundCheck::mayWrapAround(const BinaryOperator *BinOp,
                                                 ASTContext *Context) const {
  // For subtraction: always potentially dangerous with unsigned types
  // unless we can prove otherwise (which would require complex analysis)
  if (BinOp->getOpcode() == BO_Sub) {
    return true;
  }

  // For addition and multiplication: potentially dangerous
  // A complete analysis would require value range analysis, which is complex
  // For now, we flag all such operations as the rule requires
  if (BinOp->getOpcode() == BO_Add || BinOp->getOpcode() == BO_Mul) {
    return true;
  }

  return false;
}

void AvoidUnsignedWrapAroundCheck::checkBinaryOperator(
    const BinaryOperator *BinOp, ASTContext *Context) {
  // Skip if in system header
  if (Context->getSourceManager().isInSystemHeader(BinOp->getOperatorLoc()))
    return;

  // Get the types of both operands
  const Expr *LHS = BinOp->getLHS()->IgnoreParenImpCasts();
  const Expr *RHS = BinOp->getRHS()->IgnoreParenImpCasts();

  QualType LHSType = LHS->getType();
  QualType RHSType = RHS->getType();
  QualType ResultType = BinOp->getType();

  // Check if result is unsigned (wrap-around would occur)
  bool ResultIsUnsigned = isUnsignedIntegerType(ResultType);
  (void)LHSType;
  (void)RHSType;

  // Only flag if the result is unsigned (which means wrap-around would occur)
  if (!ResultIsUnsigned)
    return;

  // Skip if both operands are compile-time constants
  // (the compiler would already warn about overflow in constant expressions)
  Expr::EvalResult LHSResult, RHSResult;
  if (LHS->EvaluateAsInt(LHSResult, *Context) &&
      RHS->EvaluateAsInt(RHSResult, *Context)) {
    return;
  }

  // Check if this operation may cause wrap-around
  if (mayWrapAround(BinOp, Context)) {
    StringRef OpStr = BinOp->getOpcodeStr();

    if (BinOp->getOpcode() == BO_Sub) {
      diag(BinOp->getOperatorLoc(),
           "unsigned integer subtraction '%0' may wrap around; "
           "ensure the minuend is greater than or equal to the subtrahend, "
           "or use signed arithmetic if negative values are expected")
          << OpStr;
    } else if (BinOp->getOpcode() == BO_Add) {
      diag(BinOp->getOperatorLoc(),
           "unsigned integer addition '%0' may wrap around; "
           "ensure the result does not exceed the maximum value of the type")
          << OpStr;
    } else if (BinOp->getOpcode() == BO_Mul) {
      diag(BinOp->getOperatorLoc(),
           "unsigned integer multiplication '%0' may wrap around; "
           "ensure the result does not exceed the maximum value of the type")
          << OpStr;
    }
  }
}

void AvoidUnsignedWrapAroundCheck::checkCompoundAssignment(
    const CompoundAssignOperator *CompoundOp, ASTContext *Context) {
  // Skip if in system header
  if (Context->getSourceManager().isInSystemHeader(
          CompoundOp->getOperatorLoc()))
    return;

  // Get the LHS type (the variable being modified)
  const Expr *LHS = CompoundOp->getLHS()->IgnoreParenImpCasts();
  QualType LHSType = LHS->getType();

  // Check if LHS is unsigned integer type
  if (!isUnsignedIntegerType(LHSType))
    return;

  // Get the RHS
  const Expr *RHS = CompoundOp->getRHS()->IgnoreParenImpCasts();

  // Skip if RHS is a compile-time constant (less likely to be problematic)
  Expr::EvalResult RHSResult;
  if (RHS->EvaluateAsInt(RHSResult, *Context)) {
    // For subtraction with constant, only warn if constant is non-zero
    // (x -= 0 is safe)
    if (CompoundOp->getOpcode() == BO_SubAssign) {
      if (RHSResult.Val.getInt() == 0)
        return;
    }
    // For addition/multiplication, we might want to check if it's small
    // but for strict compliance, we warn about all cases
  }

  StringRef OpStr;
  switch (CompoundOp->getOpcode()) {
  case BO_SubAssign:
    OpStr = "-=";
    diag(CompoundOp->getOperatorLoc(),
         "unsigned integer compound assignment '%0' may cause wrap-around; "
         "ensure the operation does not underflow")
        << OpStr;
    break;
  case BO_AddAssign:
    OpStr = "+=";
    diag(CompoundOp->getOperatorLoc(),
         "unsigned integer compound assignment '%0' may cause wrap-around; "
         "ensure the operation does not overflow")
        << OpStr;
    break;
  case BO_MulAssign:
    OpStr = "*=";
    diag(CompoundOp->getOperatorLoc(),
         "unsigned integer compound assignment '%0' may cause wrap-around; "
         "ensure the operation does not overflow")
        << OpStr;
    break;
  default:
    break;
  }
}

} // namespace clang::tidy::automotive::cpp23

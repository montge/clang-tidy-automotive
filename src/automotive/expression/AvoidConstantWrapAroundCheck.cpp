//===--- AvoidConstantWrapAroundCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidConstantWrapAroundCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/APInt.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Helper to evaluate both operands of a binary expression.
/// Returns true if both operands evaluate to integer constants.
bool evaluateBinaryOperands(BinaryOperator *BO, ASTContext &Context,
                            llvm::APSInt &LHSVal, llvm::APSInt &RHSVal) {
  Expr::EvalResult LHS;
  Expr::EvalResult RHS;
  if (!BO->getLHS()->EvaluateAsRValue(LHS, Context) ||
      !BO->getRHS()->EvaluateAsRValue(RHS, Context))
    return false;

  if (!LHS.Val.isInt() || !RHS.Val.isInt())
    return false;

  LHSVal = LHS.Val.getInt();
  RHSVal = RHS.Val.getInt();
  return true;
}

/// Check if subtraction wraps around (LHS < RHS for unsigned).
bool checkSubtractionWrapAround(BinaryOperator *BO, ASTContext &Context) {
  llvm::APSInt LHSVal;
  llvm::APSInt RHSVal;
  if (!evaluateBinaryOperands(BO, Context, LHSVal, RHSVal))
    return false;

  return LHSVal.ult(RHSVal);
}

/// Check if addition wraps around (result < either operand for unsigned).
bool checkAdditionWrapAround(BinaryOperator *BO, ASTContext &Context) {
  Expr::EvalResult Result;
  if (!BO->EvaluateAsRValue(Result, Context) || !Result.Val.isInt())
    return false;

  llvm::APSInt LHSVal;
  llvm::APSInt RHSVal;
  if (!evaluateBinaryOperands(BO, Context, LHSVal, RHSVal))
    return false;

  const llvm::APSInt &ResultVal = Result.Val.getInt();
  return ResultVal.ult(LHSVal) || ResultVal.ult(RHSVal);
}

/// Check if multiplication wraps around (result/lhs != rhs).
bool checkMultiplicationWrapAround(BinaryOperator *BO, ASTContext &Context) {
  Expr::EvalResult Result;
  if (!BO->EvaluateAsRValue(Result, Context) || !Result.Val.isInt())
    return false;

  llvm::APSInt LHSVal;
  llvm::APSInt RHSVal;
  if (!evaluateBinaryOperands(BO, Context, LHSVal, RHSVal))
    return false;

  if (LHSVal.isZero() || RHSVal.isZero())
    return false;

  const llvm::APSInt &ResultVal = Result.Val.getInt();
  llvm::APInt QuotientInt = ResultVal.udiv(LHSVal);
  llvm::APSInt Quotient(QuotientInt, /*isUnsigned=*/true);
  return Quotient != RHSVal;
}

/// Visitor to check for wrap-around in binary and unary operations
class WrapAroundChecker : public RecursiveASTVisitor<WrapAroundChecker> {
public:
  explicit WrapAroundChecker(ASTContext &Context) : Context(Context) {}

  bool foundWrapAround() const { return FoundWrapAround; }
  SourceLocation getWrapAroundLoc() const { return WrapAroundLoc; }

  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (!BO->getType()->isUnsignedIntegerType())
      return true;

    bool HasWrapAround = false;
    switch (BO->getOpcode()) {
    case BO_Sub:
      HasWrapAround = checkSubtractionWrapAround(BO, Context);
      break;
    case BO_Add:
      HasWrapAround = checkAdditionWrapAround(BO, Context);
      break;
    case BO_Mul:
      HasWrapAround = checkMultiplicationWrapAround(BO, Context);
      break;
    default:
      return true;
    }

    if (HasWrapAround) {
      FoundWrapAround = true;
      WrapAroundLoc = BO->getOperatorLoc();
      return false;
    }
    return true;
  }

  bool VisitUnaryOperator(UnaryOperator *UO) {
    if (UO->getOpcode() != UO_Minus)
      return true;
    if (!UO->getType()->isUnsignedIntegerType())
      return true;

    Expr::EvalResult Operand;
    if (!UO->getSubExpr()->EvaluateAsRValue(Operand, Context))
      return true;

    if (Operand.Val.isInt() && !Operand.Val.getInt().isZero()) {
      FoundWrapAround = true;
      WrapAroundLoc = UO->getOperatorLoc();
      return false;
    }
    return true;
  }

private:
  ASTContext &Context;
  bool FoundWrapAround = false;
  SourceLocation WrapAroundLoc;
};

} // anonymous namespace

void AvoidConstantWrapAroundCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with initializers that are constant expressions
  Finder->addMatcher(
      varDecl(hasInitializer(expr().bind("init")), isDefinition()).bind("var"),
      this);
}

void AvoidConstantWrapAroundCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *InitExpr = Result.Nodes.getNodeAs<Expr>("init");

  if (!VD || !InitExpr)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  // Only check const variables or those in file scope (likely constants)
  if (!VD->getType().isConstQualified() && !VD->hasGlobalStorage())
    return;

  // Only check unsigned integer types
  QualType Type = VD->getType();
  if (!Type->isUnsignedIntegerType())
    return;

  // Check if the initializer can be evaluated as a constant
  Expr::EvalResult EvalResult;
  if (!InitExpr->EvaluateAsRValue(EvalResult, *Result.Context))
    return;

  // Walk the initializer to find wrap-around operations
  WrapAroundChecker Checker(*Result.Context);
  Checker.TraverseStmt(const_cast<Expr *>(InitExpr));  // NOSONAR(S859): const_cast required by RecursiveASTVisitor API

  if (Checker.foundWrapAround()) {
    diag(
        Checker.getWrapAroundLoc(),
        "constant expression evaluation leads to unsigned integer wrap-around");
  }
}

} // namespace clang::tidy::automotive

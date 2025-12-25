//===--- AvoidNonArrayPointerArithCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNonArrayPointerArithCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidNonArrayPointerArithCheck::registerMatchers(MatchFinder *Finder) {
  // Match binary operations: pointer + integer or integer + pointer
  Finder->addMatcher(
      binaryOperator(
          anyOf(hasOperatorName("+"), hasOperatorName("-"),
                hasOperatorName("+="), hasOperatorName("-=")),
          hasEitherOperand(ignoringImpCasts(
              unaryOperator(
                  hasOperatorName("&"),
                  hasUnaryOperand(declRefExpr(
                      to(varDecl(unless(hasType(arrayType()))).bind("var")))))
                  .bind("addr_of"))))
          .bind("binary_op"),
      this);

  // Match pointer arithmetic on new T (single object)
  Finder->addMatcher(
      binaryOperator(anyOf(hasOperatorName("+"), hasOperatorName("-"),
                           hasOperatorName("+="), hasOperatorName("-=")),
                     hasEitherOperand(ignoringImpCasts(
                         cxxNewExpr(unless(isArray())).bind("new_expr"))))
          .bind("new_arith"),
      this);
}

void AvoidNonArrayPointerArithCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Handle binary operations on address-of non-array variable
  if (const auto *BinOp = Result.Nodes.getNodeAs<BinaryOperator>("binary_op")) {
    if (Result.Nodes.getNodeAs<UnaryOperator>("addr_of")) {
      if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
        diag(BinOp->getOperatorLoc(),
             "pointer arithmetic applied to address of non-array variable %0")
            << Var;
        diag(Var->getLocation(), "variable declared here", DiagnosticIDs::Note);
        return;
      }
    }
  }

  // Handle pointer arithmetic on new expression (single object)
  if (const auto *NewArith =
          Result.Nodes.getNodeAs<BinaryOperator>("new_arith")) {
    if (const auto *NewExpr = Result.Nodes.getNodeAs<CXXNewExpr>("new_expr")) {
      diag(NewArith->getOperatorLoc(),
           "pointer arithmetic applied to single object allocated with 'new'");
      diag(NewExpr->getBeginLoc(), "single object allocated here",
           DiagnosticIDs::Note);
      return;
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

//===--- AvoidNullLiteralAssignmentCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNullLiteralAssignmentCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
// Check if this is a null pointer literal (0, 0L, NULL, etc.) but not nullptr
AST_MATCHER(IntegerLiteral, isNullPointerLiteral) {
  return Node.getValue() == 0;
}
} // namespace

void AvoidNullLiteralAssignmentCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with pointer type initialized with 0
  Finder->addMatcher(varDecl(hasType(pointerType()),
                             hasInitializer(ignoringImpCasts(
                                 integerLiteral(isNullPointerLiteral()))))
                         .bind("var_init"),
                     this);

  // Match assignments to pointer type with 0
  Finder->addMatcher(
      binaryOperator(
          isAssignmentOperator(), hasLHS(hasType(pointerType())),
          hasRHS(ignoringImpCasts(integerLiteral(isNullPointerLiteral()))))
          .bind("assign"),
      this);

  // Match function calls where pointer parameter receives 0
  Finder->addMatcher(
      callExpr(
          forEachArgumentWithParam(
              ignoringImpCasts(
                  integerLiteral(isNullPointerLiteral()).bind("arg_literal")),
              parmVarDecl(hasType(pointerType()))))
          .bind("call"),
      this);

  // Match return statements returning 0 in pointer-returning functions
  Finder->addMatcher(returnStmt(hasReturnValue(ignoringImpCasts(
                                    integerLiteral(isNullPointerLiteral()))))
                         .bind("return"),
                     this);
}

void AvoidNullLiteralAssignmentCheck::check(
    const MatchFinder::MatchResult &Result) {
  SourceLocation Loc;
  StringRef Context;

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var_init")) {
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;
    // Get the initializer expression location
    if (const auto *Init = VD->getInit()) {
      if (const auto *ICE = dyn_cast<ImplicitCastExpr>(Init)) {
        if (const auto *IL = dyn_cast<IntegerLiteral>(ICE->getSubExpr())) {
          Loc = IL->getLocation();
        }
      } else if (const auto *IL = dyn_cast<IntegerLiteral>(Init)) {
        Loc = IL->getLocation();
      }
    }
    if (Loc.isInvalid())
      Loc = VD->getLocation();
    Context = "initializing pointer";
  } else if (const auto *BO =
                 Result.Nodes.getNodeAs<BinaryOperator>("assign")) {
    if (Result.SourceManager->isInSystemHeader(BO->getOperatorLoc()))
      return;
    if (const auto *RHS = BO->getRHS()) {
      const Expr *E = RHS->IgnoreImpCasts();
      if (const auto *IL = dyn_cast<IntegerLiteral>(E)) {
        Loc = IL->getLocation();
      }
    }
    if (Loc.isInvalid())
      Loc = BO->getOperatorLoc();
    Context = "assigning to pointer";
  } else if (const auto *ArgLiteral =
                 Result.Nodes.getNodeAs<IntegerLiteral>("arg_literal")) {
    if (Result.SourceManager->isInSystemHeader(ArgLiteral->getLocation()))
      return;
    Loc = ArgLiteral->getLocation();
    Context = "passing to pointer parameter";
  } else if (const auto *RS = Result.Nodes.getNodeAs<ReturnStmt>("return")) {
    // Check if we're in a function returning a pointer
    const auto &Parents = Result.Context->getParents(*RS);
    const FunctionDecl *FD = nullptr;
    for (const auto &Parent : Parents) {
      if (const auto *CS = Parent.get<CompoundStmt>()) {
        const auto &GrandParents = Result.Context->getParents(*CS);
        for (const auto &GP : GrandParents) {
          FD = GP.get<FunctionDecl>();
          if (FD)
            break;
        }
      }
      if (FD)
        break;
    }

    // Walk up to find FunctionDecl
    const Stmt *Current = RS;
    while (!FD) {
      const auto &Parents2 = Result.Context->getParents(*Current);
      if (Parents2.empty())
        break;
      const auto &Parent = Parents2[0];
      if (const auto *F = Parent.get<FunctionDecl>()) {
        FD = F;
        break;
      }
      if (const auto *S = Parent.get<Stmt>()) {
        Current = S;
      } else {
        break;
      }
    }

    if (!FD || !FD->getReturnType()->isPointerType())
      return;

    if (Result.SourceManager->isInSystemHeader(RS->getReturnLoc()))
      return;

    if (const auto *RetVal = RS->getRetValue()) {
      const Expr *E = RetVal->IgnoreImpCasts();
      if (const auto *IL = dyn_cast<IntegerLiteral>(E)) {
        Loc = IL->getLocation();
      }
    }
    if (Loc.isInvalid())
      Loc = RS->getReturnLoc();
    Context = "returning from pointer function";
  }

  if (Loc.isValid()) {
    diag(Loc, "use 'nullptr' instead of literal '0' when %0; "
              "integer literals shall not be used as null pointer constants")
        << Context;
  }
}

} // namespace clang::tidy::automotive

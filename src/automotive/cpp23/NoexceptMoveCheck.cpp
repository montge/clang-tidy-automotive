//===--- NoexceptMoveCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoexceptMoveCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void NoexceptMoveCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus11)
    return;

  // Match move constructors
  Finder->addMatcher(cxxConstructorDecl(isMoveConstructor(),
                                        unless(isImplicit()),
                                        unless(isDeleted()))
                         .bind("move_ctor"),
                     this);

  // Match move assignment operators
  Finder->addMatcher(cxxMethodDecl(isMoveAssignmentOperator(),
                                   unless(isImplicit()), unless(isDeleted()))
                         .bind("move_assign"),
                     this);
}

void NoexceptMoveCheck::check(const MatchFinder::MatchResult &Result) {
  const FunctionDecl *Func = nullptr;
  bool IsMoveConstructor = false;

  if (const auto *MoveCtor =
          Result.Nodes.getNodeAs<CXXConstructorDecl>("move_ctor")) {
    Func = MoveCtor;
    IsMoveConstructor = true;
  } else if (const auto *MoveAssign =
                 Result.Nodes.getNodeAs<CXXMethodDecl>("move_assign")) {
    Func = MoveAssign;
    IsMoveConstructor = false;
  }

  if (!Func)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Func->getLocation()))
    return;

  // Check if the function is noexcept
  const auto *FPT = Func->getType()->getAs<FunctionProtoType>();
  if (!FPT)
    return;

  // Check exception specification
  ExceptionSpecificationType EST = FPT->getExceptionSpecType();

  // noexcept or noexcept(true) is acceptable
  if (EST == EST_BasicNoexcept || EST == EST_NoexceptTrue)
    return;

  // noexcept(false) or throw() specifications are not acceptable
  // Also catch functions with no exception specification

  const char *FuncType =
      IsMoveConstructor ? "move constructor" : "move assignment operator";

  diag(Func->getLocation(), "%0 should be declared noexcept") << FuncType;
}

} // namespace clang::tidy::automotive

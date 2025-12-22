//===--- MissingReturnCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingReturnCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Check if a statement unconditionally returns or throws
bool alwaysReturns(const Stmt *S);

bool alwaysReturnsFromCompound(const CompoundStmt *CS) {
  if (CS->body_empty())
    return false;

  // Check if the last statement returns
  const Stmt *LastStmt = CS->body_back();
  return alwaysReturns(LastStmt);
}

bool alwaysReturns(const Stmt *S) {
  if (!S)
    return false;

  // Direct return statement
  if (isa<ReturnStmt>(S))
    return true;

  // Throw statement
  if (isa<CXXThrowExpr>(S))
    return true;

  // Compound statement - check if it ends with return
  if (const auto *CS = dyn_cast<CompoundStmt>(S))
    return alwaysReturnsFromCompound(CS);

  // If statement - both branches must return
  if (const auto *If = dyn_cast<IfStmt>(S)) {
    const Stmt *Then = If->getThen();
    const Stmt *Else = If->getElse();

    // If there's no else, we can't guarantee all paths return
    if (!Else)
      return false;

    return alwaysReturns(Then) && alwaysReturns(Else);
  }

  // Switch statement - harder to analyze, be conservative
  // Would need to check all cases and default

  // While/Do/For loops - they might not execute, so can't rely on return inside

  return false;
}

/// Check if a function body always returns
bool functionBodyAlwaysReturns(const FunctionDecl *FD) {
  const Stmt *Body = FD->getBody();
  if (!Body)
    return true; // No body means declaration only

  return alwaysReturns(Body);
}

} // anonymous namespace

void MissingReturnCheck::registerMatchers(MatchFinder *Finder) {
  // Match function definitions with non-void return type
  Finder->addMatcher(functionDecl(isDefinition(), unless(returns(voidType())),
                                  unless(isImplicit()),
                                  unless(hasAttr(attr::NoReturn)))
                         .bind("function"),
                     this);
}

void MissingReturnCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("function");
  if (!FD)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
    return;

  // Skip main function (implicit return 0)
  if (FD->isMain())
    return;

  // Skip constructors and destructors (handled separately)
  if (isa<CXXConstructorDecl>(FD) || isa<CXXDestructorDecl>(FD))
    return;

  // Check if the function body always returns
  if (!functionBodyAlwaysReturns(FD)) {
    diag(FD->getLocation(),
         "non-void function '%0' does not return a value on all control paths")
        << FD->getName();
  }
}

} // namespace clang::tidy::automotive

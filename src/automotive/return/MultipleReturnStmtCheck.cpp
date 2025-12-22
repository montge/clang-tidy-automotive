//===--- MultipleReturnStmtCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MultipleReturnStmtCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/SmallVector.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MultipleReturnStmtCheck::registerMatchers(MatchFinder *Finder) {
  // Match function declarations with bodies
  Finder->addMatcher(functionDecl(unless(isExpansionInSystemHeader()),
                                  isDefinition(), unless(isDefaulted()),
                                  unless(isDeleted()))
                         .bind("func"),
                     this);
}

void MultipleReturnStmtCheck::collectReturnStmts(
    const Stmt *S, llvm::SmallVectorImpl<const ReturnStmt *> &Returns) {
  if (!S)
    return;

  if (const auto *RS = dyn_cast<ReturnStmt>(S)) {
    Returns.push_back(RS);
    return;
  }

  // Don't descend into nested lambdas or local functions
  if (isa<LambdaExpr>(S))
    return;

  for (const Stmt *Child : S->children()) {
    collectReturnStmts(Child, Returns);
  }
}

void MultipleReturnStmtCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!Func || !Func->getBody())
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Func->getLocation()))
    return;

  // Collect all return statements
  llvm::SmallVector<const ReturnStmt *, 8> Returns;
  collectReturnStmts(Func->getBody(), Returns);

  // Only report if there are multiple returns
  if (Returns.size() <= 1)
    return;

  // Report the function first
  diag(Func->getLocation(), "function has multiple return statements")
      << Func->getNameAsString();

  // Report each return statement
  for (const ReturnStmt *RS : Returns) {
    diag(RS->getBeginLoc(), "return statement here", DiagnosticIDs::Note);
  }
}

} // namespace clang::tidy::automotive

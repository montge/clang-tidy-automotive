//===--- SinglePointOfExitCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SinglePointOfExitCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {

class ReturnCounter : public RecursiveASTVisitor<ReturnCounter> {
public:
  unsigned Count = 0;
  SmallVector<const ReturnStmt *, 4> Returns;

  bool VisitReturnStmt(ReturnStmt *RS) {
    Count++;
    Returns.push_back(RS);
    return true;
  }

  // Don't descend into nested functions/lambdas
  bool TraverseLambdaExpr(LambdaExpr *) { return true; }
  bool TraverseBlockExpr(BlockExpr *) { return true; }
};

} // namespace

void SinglePointOfExitCheck::registerMatchers(MatchFinder *Finder) {
  // Match function definitions (not just declarations)
  // Exclude lambda call operators (they're handled separately if needed)
  Finder->addMatcher(
      functionDecl(isDefinition(), hasBody(stmt()),
                   unless(cxxMethodDecl(ofClass(cxxRecordDecl(isLambda())))))
          .bind("function"),
      this);
}

void SinglePointOfExitCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("function");
  if (!Func)
    return;

  // Skip main function - it's often special
  if (Func->isMain())
    return;

  // Skip deleted/defaulted functions
  if (Func->isDeleted() || Func->isDefaulted())
    return;

  // Skip trivial functions (like getters that just return a value)
  const Stmt *Body = Func->getBody();
  if (!Body)
    return;

  ReturnCounter Counter;
  Counter.TraverseStmt(const_cast<Stmt *>(Body));

  // Only warn if there are multiple return statements
  if (Counter.Count > 1) {
    diag(Func->getLocation(),
         "function '%0' has %1 return statements; consider using a single "
         "point of exit")
        << Func->getName() << Counter.Count;

    // Add notes for each return statement
    for (const auto *RS : Counter.Returns) {
      diag(RS->getReturnLoc(), "return statement here", DiagnosticIDs::Note);
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

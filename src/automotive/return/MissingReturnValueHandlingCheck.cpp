//===--- MissingReturnValueHandlingCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingReturnValueHandlingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MissingReturnValueHandlingCheck::registerMatchers(MatchFinder *Finder) {
  // Match call expressions where return value is discarded:
  // - Direct child of compound statement: { foo(); }
  // - In for-loop init/increment: for(foo(); ...; bar())
  // - In expression statement context via comma operator: foo(), bar();
  Finder->addMatcher(
      callExpr(
          unless(isExpansionInSystemHeader()),
          callee(functionDecl(unless(returns(voidType())))),
          // The call is used as a statement, not as part of an expression
          hasParent(stmt(anyOf(compoundStmt(), forStmt(), exprWithCleanups(),
                               materializeTemporaryExpr()))))
          .bind("missingReturn"),
      this);

  // Also match calls inside expression statements (top-level expression)
  Finder->addMatcher(
      callExpr(
          unless(isExpansionInSystemHeader()),
          callee(functionDecl(unless(returns(voidType())))),
          // The call is cast to void - explicit discard is OK, skip
          unless(hasParent(cStyleCastExpr(hasDestinationType(voidType())))),
          unless(hasParent(cxxStaticCastExpr(hasDestinationType(voidType())))),
          // Match calls that are the LHS of comma operator (discarded)
          hasParent(binaryOperator(hasOperatorName(","))))
          .bind("commaDiscarded"),
      this);
}

void MissingReturnValueHandlingCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("missingReturn");
  if (!Call)
    Call = Result.Nodes.getNodeAs<CallExpr>("commaDiscarded");

  if (!Call)
    return;

  // Skip if in system header (double check)
  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  // Skip if the call is inside a macro
  if (Call->getBeginLoc().isMacroID())
    return;

  diag(Call->getBeginLoc(), "returned value from function is not used");
}

} // namespace clang::tidy::automotive

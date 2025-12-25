//===--- AvoidDiscardedSideEffectsCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidDiscardedSideEffectsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidDiscardedSideEffectsCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to [[nodiscard]] functions in unused expression statements
  Finder->addMatcher(
      callExpr(hasParent(exprWithCleanups(hasParent(compoundStmt()))),
               callee(functionDecl(hasAttr(attr::WarnUnusedResult))
                          .bind("nodiscard_func")))
          .bind("nodiscard_call"),
      this);

  // Also match calls to nodiscard in expression statements directly
  Finder->addMatcher(
      exprWithCleanups(
          hasParent(compoundStmt()),
          has(callExpr(callee(functionDecl(hasAttr(attr::WarnUnusedResult))
                                  .bind("nodiscard_func2")))
                  .bind("nodiscard_call2"))),
      this);

  // Match explicit void casts of function calls
  Finder->addMatcher(cStyleCastExpr(hasDestinationType(voidType()),
                                    has(callExpr().bind("void_cast_call")))
                         .bind("void_cast"),
                     this);

  // Also match C++ style cast to void
  Finder->addMatcher(
      cxxStaticCastExpr(hasDestinationType(voidType()),
                        has(callExpr().bind("void_cast_call_cxx")))
          .bind("void_cast_cxx"),
      this);

  // Match comma operator where left operand is a function call
  Finder->addMatcher(
      binaryOperator(hasOperatorName(","), hasLHS(ignoringParenImpCasts(
                                               callExpr().bind("comma_call"))))
          .bind("comma_op"),
      this);
}

void AvoidDiscardedSideEffectsCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Handle nodiscard function calls that are unused
  if (const auto *Call = Result.Nodes.getNodeAs<CallExpr>("nodiscard_call")) {
    if (const auto *Func =
            Result.Nodes.getNodeAs<FunctionDecl>("nodiscard_func")) {
      diag(Call->getBeginLoc(),
           "return value of function %0 marked [[nodiscard]] is ignored")
          << Func;
      diag(Func->getLocation(), "function declared here", DiagnosticIDs::Note);
    }
    return;
  }

  if (const auto *Call = Result.Nodes.getNodeAs<CallExpr>("nodiscard_call2")) {
    if (const auto *Func =
            Result.Nodes.getNodeAs<FunctionDecl>("nodiscard_func2")) {
      diag(Call->getBeginLoc(),
           "return value of function %0 marked [[nodiscard]] is ignored")
          << Func;
      diag(Func->getLocation(), "function declared here", DiagnosticIDs::Note);
    }
    return;
  }

  // Handle explicit void casts
  if (const auto *VoidCast =
          Result.Nodes.getNodeAs<CStyleCastExpr>("void_cast")) {
    if (const auto *Call = Result.Nodes.getNodeAs<CallExpr>("void_cast_call")) {
      diag(VoidCast->getBeginLoc(),
           "explicit cast to void discards function return value; side effects "
           "should not be discarded")
          << FixItHint::CreateRemoval(CharSourceRange::getCharRange(
                 VoidCast->getBeginLoc(), Call->getBeginLoc()));
    }
    return;
  }

  if (const auto *VoidCast =
          Result.Nodes.getNodeAs<CXXStaticCastExpr>("void_cast_cxx")) {
    if (const auto *Call =
            Result.Nodes.getNodeAs<CallExpr>("void_cast_call_cxx")) {
      diag(VoidCast->getBeginLoc(),
           "explicit cast to void discards function return value; side effects "
           "should not be discarded");
    }
    return;
  }

  // Handle comma operator discarding results
  if (const auto *CommaOp =
          Result.Nodes.getNodeAs<BinaryOperator>("comma_op")) {
    if (const auto *Call = Result.Nodes.getNodeAs<CallExpr>("comma_call")) {
      // Only flag if the function returns non-void
      QualType ReturnType = Call->getCallReturnType(*Result.Context);
      if (!ReturnType.isNull() && !ReturnType->isVoidType()) {
        diag(Call->getBeginLoc(), "comma operator discards function return "
                                  "value; side effects should "
                                  "not be discarded")
            << CommaOp->getSourceRange();
      }
    }
    return;
  }
}

} // namespace clang::tidy::automotive::cpp23

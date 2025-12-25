//===--- UseNullptrCheck.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseNullptrCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {

// Matcher to identify null pointer literals (integer 0 in pointer context)
AST_MATCHER(IntegerLiteral, isZero) { return Node.getValue() == 0; }

// Check if an expression is a macro expansion of NULL
bool isNULLMacro(const Expr *E, const SourceManager &SM,
                 const LangOptions &LangOpts) {
  if (!E)
    return false;

  SourceLocation Loc = E->getBeginLoc();
  if (!Loc.isMacroID())
    return false;

  StringRef MacroName = Lexer::getImmediateMacroName(Loc, SM, LangOpts);
  return MacroName == "NULL";
}

} // namespace

void UseNullptrCheck::registerMatchers(MatchFinder *Finder) {
  // Match implicit casts from integer to pointer (catches 0 and NULL in all
  // contexts except comparisons, which we handle separately)
  Finder->addMatcher(
      implicitCastExpr(hasCastKind(CK_NullToPointer),
                       unless(hasSourceExpression(cxxNullPtrLiteralExpr())),
                       unless(hasParent(binaryOperator(anyOf(
                           hasOperatorName("=="), hasOperatorName("!="))))),
                       hasSourceExpression(expr().bind("null_expr")))
          .bind("cast"),
      this);

  // Match explicit comparisons with 0 or NULL where one side is a pointer
  Finder->addMatcher(
      binaryOperator(
          anyOf(hasOperatorName("=="), hasOperatorName("!=")),
          eachOf(allOf(hasLHS(hasType(pointerType())),
                       hasRHS(ignoringImpCasts(integerLiteral(isZero()).bind(
                           "comparison_literal")))),
                 allOf(hasRHS(hasType(pointerType())),
                       hasLHS(ignoringImpCasts(integerLiteral(isZero()).bind(
                           "comparison_literal"))))))
          .bind("comparison"),
      this);
}

void UseNullptrCheck::check(const MatchFinder::MatchResult &Result) {
  const SourceManager &SM = *Result.SourceManager;
  const LangOptions &LangOpts = Result.Context->getLangOpts();

  // Handle implicit casts (assignments, initializations, function arguments,
  // returns)
  if (const auto *Cast = Result.Nodes.getNodeAs<ImplicitCastExpr>("cast")) {
    if (SM.isInSystemHeader(Cast->getBeginLoc()))
      return;

    const auto *NullExpr = Result.Nodes.getNodeAs<Expr>("null_expr");
    if (!NullExpr)
      return;

    SourceLocation Loc = NullExpr->getBeginLoc();

    // Check if this is NULL macro
    if (isNULLMacro(NullExpr, SM, LangOpts)) {
      diag(Loc, "use 'nullptr' instead of NULL macro; nullptr shall be the "
                "only null pointer constant (MISRA C++:2023 Rule 7.11.2)");
      return;
    }

    // Check if this is integer literal 0
    if (const auto *IL =
            dyn_cast<IntegerLiteral>(NullExpr->IgnoreParenImpCasts())) {
      if (IL->getValue() == 0) {
        diag(Loc,
             "use 'nullptr' instead of integer literal '0'; nullptr shall "
             "be the only null pointer constant (MISRA C++:2023 Rule 7.11.2)");
        return;
      }
    }
  }

  // Handle explicit comparisons with 0
  if (const auto *CompLiteral =
          Result.Nodes.getNodeAs<IntegerLiteral>("comparison_literal")) {
    if (SM.isInSystemHeader(CompLiteral->getBeginLoc()))
      return;

    SourceLocation Loc = CompLiteral->getBeginLoc();

    // Check if it's from NULL macro
    if (Loc.isMacroID()) {
      StringRef MacroName = Lexer::getImmediateMacroName(Loc, SM, LangOpts);
      if (MacroName == "NULL") {
        diag(Loc, "use 'nullptr' instead of NULL macro in pointer comparison; "
                  "nullptr shall be the only null pointer constant "
                  "(MISRA C++:2023 Rule 7.11.2)");
        return;
      }
    }

    diag(Loc, "use 'nullptr' instead of integer literal '0' in pointer "
              "comparison; nullptr shall be the only null pointer constant "
              "(MISRA C++:2023 Rule 7.11.2)");
  }
}

} // namespace clang::tidy::automotive::cpp23

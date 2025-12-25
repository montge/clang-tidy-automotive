//===--- GenericSideEffectCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericSideEffectCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Visitor to detect expressions with potential side effects
class SideEffectFinder : public RecursiveASTVisitor<SideEffectFinder> {
public:
  bool hasSideEffect() const { return FoundSideEffect; }
  SourceLocation getSideEffectLoc() const { return SideEffectLoc; }
  StringRef getSideEffectKind() const { return SideEffectKind; }

  bool VisitUnaryOperator(UnaryOperator *UO) {
    if (UO->isIncrementDecrementOp()) {
      FoundSideEffect = true;
      SideEffectLoc = UO->getOperatorLoc();
      SideEffectKind = UO->isIncrementOp() ? "increment" : "decrement";
      return false;
    }
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (BO->isAssignmentOp()) {
      FoundSideEffect = true;
      SideEffectLoc = BO->getOperatorLoc();
      SideEffectKind = "assignment";
      return false;
    }
    return true;
  }

  bool VisitCompoundAssignOperator(CompoundAssignOperator *CAO) {
    FoundSideEffect = true;
    SideEffectLoc = CAO->getOperatorLoc();
    SideEffectKind = "compound assignment";
    return false;
  }

  bool VisitCallExpr(CallExpr *CE) {
    FoundSideEffect = true;
    SideEffectLoc = CE->getBeginLoc();
    SideEffectKind = "function call";
    return false;
  }

private:
  bool FoundSideEffect = false;
  SourceLocation SideEffectLoc;
  StringRef SideEffectKind;
};

} // anonymous namespace

void GenericSideEffectCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(genericSelectionExpr().bind("generic"), this);
}

void GenericSideEffectCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Generic = Result.Nodes.getNodeAs<GenericSelectionExpr>("generic");
  if (!Generic)
    return;

  SourceLocation Loc = Generic->getGenericLoc();

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Loc))
    return;

  // Get the controlling expression
  const Expr *ControlExpr = Generic->getControllingExpr();
  if (!ControlExpr)
    return;

  // Check if the controlling expression is from a macro argument
  // If it is, side effects are allowed (they would be evaluated in the
  // macro expansion context)
  SourceLocation ControlLoc = ControlExpr->getBeginLoc();
  if (ControlLoc.isMacroID()) {
    // The expression comes from a macro - check if it's a macro argument
    // If the spelling location differs from expansion location, it's likely
    // a macro argument
    SourceLocation SpellingLoc =
        Result.SourceManager->getSpellingLoc(ControlLoc);
    SourceLocation ExpansionLoc =
        Result.SourceManager->getExpansionLoc(ControlLoc);
    if (SpellingLoc != ExpansionLoc) {
      // This is from a macro argument, allow side effects
      return;
    }
  }

  // Check for side effects in the controlling expression
  SideEffectFinder Finder;
  Finder.TraverseStmt(const_cast<Expr *>(ControlExpr));

  if (Finder.hasSideEffect()) {
    diag(Finder.getSideEffectLoc(),
         "controlling expression of _Generic contains %0 which will not be "
         "evaluated; the expression is only used for its type")
        << Finder.getSideEffectKind();
  }
}

} // namespace clang::tidy::automotive

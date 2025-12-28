//===--- UnreachableCodeCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnreachableCodeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void UnreachableCodeCheck::registerMatchers(MatchFinder *Finder) {
  // Match compound statements (blocks of code)
  Finder->addMatcher(
      compoundStmt(unless(isExpansionInSystemHeader())).bind("compound"), this);
}

void UnreachableCodeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Compound = Result.Nodes.getNodeAs<CompoundStmt>("compound");
  if (!Compound)
    return;

  bool FoundTerminator = false;
  SourceLocation TerminatorLoc;
  StringRef TerminatorKind;

  for (const Stmt *S : Compound->body()) {
    if (!S)
      continue;

    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(S->getBeginLoc()))
      continue;

    // If we've seen a terminator, everything after is unreachable
    if (FoundTerminator) {
      // Skip null statements (just semicolons)
      if (isa<NullStmt>(S))
        continue;

      // Report the unreachable code
      diag(S->getBeginLoc(),
           "unreachable code after '%0' statement")
          << TerminatorKind;
      diag(TerminatorLoc, "control flow terminated here", DiagnosticIDs::Note);

      // Only report once per block to avoid noise
      return;
    }

    // Check if this statement is an unconditional terminator
    if (isa<ReturnStmt>(S)) {
      FoundTerminator = true;
      TerminatorLoc = S->getBeginLoc();
      TerminatorKind = "return";
    } else if (isa<BreakStmt>(S)) {
      FoundTerminator = true;
      TerminatorLoc = S->getBeginLoc();
      TerminatorKind = "break";
    } else if (isa<ContinueStmt>(S)) {
      FoundTerminator = true;
      TerminatorLoc = S->getBeginLoc();
      TerminatorKind = "continue";
    } else if (isa<GotoStmt>(S)) {
      FoundTerminator = true;
      TerminatorLoc = S->getBeginLoc();
      TerminatorKind = "goto";
    } else if (isa<CXXThrowExpr>(S)) {
      FoundTerminator = true;
      TerminatorLoc = S->getBeginLoc();
      TerminatorKind = "throw";
    } else if (const auto *ExprS = dyn_cast<Expr>(S)) {
      // Check for throw expressions wrapped in expression statements
      if (const auto *Throw =
              dyn_cast<CXXThrowExpr>(ExprS->IgnoreParenImpCasts())) {
        FoundTerminator = true;
        TerminatorLoc = Throw->getBeginLoc();
        TerminatorKind = "throw";
      }
    }
  }
}

} // namespace clang::tidy::automotive

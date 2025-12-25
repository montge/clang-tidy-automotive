//===--- GenericFromMacroCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericFromMacroCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void GenericFromMacroCheck::registerMatchers(MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("genericSelection"), this);
}

void GenericFromMacroCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  SourceLocation Loc = GenericSelection->getGenericLoc();

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Loc))
    return;

  // Check if the _Generic keyword location is from a macro expansion
  // If it's NOT from a macro (i.e., it's written directly in the source),
  // then it violates the rule
  if (!Loc.isMacroID()) {
    diag(Loc, "generic selection should only be expanded from a macro, not "
              "used directly in source code");
  }
}

} // namespace clang::tidy::automotive

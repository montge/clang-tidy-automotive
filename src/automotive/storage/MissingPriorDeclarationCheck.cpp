//===--- MissingPriorDeclarationCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingPriorDeclarationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MissingPriorDeclarationCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable definitions with external linkage
  Finder->addMatcher(varDecl(isDefinition(), hasExternalFormalLinkage(),
                             unless(isExpansionInSystemHeader()))
                         .bind("var"),
                     this);

  // Match function definitions with external linkage
  Finder->addMatcher(functionDecl(isDefinition(), hasExternalFormalLinkage(),
                                  unless(isExpansionInSystemHeader()))
                         .bind("func"),
                     this);
}

void MissingPriorDeclarationCheck::check(
    const MatchFinder::MatchResult &Result) {

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;

    // Check if this is the first declaration (no prior declaration exists)
    const VarDecl *FirstDecl = VD->getFirstDecl();
    if (FirstDecl == VD) {
      // This definition is also the first declaration - no prior declaration
      diag(VD->getLocation(),
           "external variable '%0' defined without a prior declaration")
          << VD->getName();
    }
  } else if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
      return;

    // Skip main function
    if (FD->isMain())
      return;

    // Skip implicit functions (compiler-generated)
    if (FD->isImplicit())
      return;

    // Check if this is the first declaration (no prior declaration exists)
    const FunctionDecl *FirstDecl = FD->getFirstDecl();
    if (FirstDecl == FD) {
      // This definition is also the first declaration - no prior declaration
      diag(FD->getLocation(),
           "external function '%0' defined without a prior declaration")
          << FD->getName();
    }
  }
}

} // namespace clang::tidy::automotive

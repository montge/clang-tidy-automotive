//===--- MissingStaticForInternalCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingStaticForInternalCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MissingStaticForInternalCheck::registerMatchers(MatchFinder *Finder) {
  // Match function definitions without static that are not main
  // In C, functions without static have external linkage by default, but if
  // they have no prior declaration in a header, they should be static.
  Finder->addMatcher(functionDecl(isDefinition(), unless(isMain()),
                                  unless(isStaticStorageClass()),
                                  unless(isInline()),
                                  hasParent(translationUnitDecl()))
                         .bind("func"),
                     this);

  // Match file-scope variable definitions without static at file scope
  // Note: isExternC() matches all C declarations (C linkage), so don't use it
  Finder->addMatcher(varDecl(hasGlobalStorage(), unless(isStaticStorageClass()),
                             hasParent(translationUnitDecl()))
                         .bind("var"),
                     this);
}

void MissingStaticForInternalCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Check function declarations
  if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
      return;

    // Skip functions that have a prior declaration (likely in header)
    if (FD->getPreviousDecl())
      return;

    // Skip inline functions (they may need external linkage for ODR)
    if (FD->isInlined())
      return;

    diag(FD->getLocation(),
         "function %0 has no external declaration; consider declaring it "
         "static for internal linkage")
        << FD;
    return;
  }

  // Check variable declarations
  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;

    // Skip extern declarations
    if (VD->hasExternalStorage())
      return;

    // Skip variables that have a prior declaration
    if (VD->getPreviousDecl())
      return;

    diag(VD->getLocation(),
         "file-scope object %0 has no external declaration; consider "
         "declaring it static for internal linkage")
        << VD;
  }
}

} // namespace clang::tidy::automotive

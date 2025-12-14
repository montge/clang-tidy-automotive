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
  Finder->addMatcher(functionDecl(isDefinition(), unless(isMain()),
                                  unless(isStaticStorageClass()),
                                  unless(isInline()),
                                  unless(hasExternalFormalLinkage()),
                                  hasParent(translationUnitDecl()))
                         .bind("func"),
                     this);

  // Match file-scope variable definitions without static
  Finder->addMatcher(varDecl(hasGlobalStorage(), unless(isStaticStorageClass()),
                             unless(isExternC()),
                             unless(hasExternalFormalLinkage()),
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

    // Skip functions with external linkage
    if (FD->getFormalLinkage() == Linkage::External)
      return;

    // Skip functions that have a prior declaration (might be in header)
    if (FD->getPreviousDecl())
      return;

    // Skip inline functions (they may need external linkage)
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

    // Skip variables with external linkage
    if (VD->getFormalLinkage() == Linkage::External)
      return;

    // Skip variables that have a prior declaration
    if (VD->getPreviousDecl())
      return;

    // Skip const variables (they have internal linkage by default in C++)
    // but not in C, so we still flag them
    diag(VD->getLocation(),
         "file-scope object %0 has no external declaration; consider "
         "declaring it static for internal linkage")
        << VD;
  }
}

} // namespace clang::tidy::automotive

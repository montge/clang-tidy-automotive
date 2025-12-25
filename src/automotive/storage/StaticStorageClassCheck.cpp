//===--- StaticStorageClassCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StaticStorageClassCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/Linkage.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void StaticStorageClassCheck::registerMatchers(MatchFinder *Finder) {
  // Match function declarations (both declarations and definitions)
  // We want to find functions that have internal linkage
  Finder->addMatcher(functionDecl(hasParent(translationUnitDecl()),
                                  unless(isExpansionInSystemHeader()))
                         .bind("func"),
                     this);

  // Match file-scope variable declarations
  Finder->addMatcher(varDecl(hasGlobalStorage(),
                             hasParent(translationUnitDecl()),
                             unless(isExpansionInSystemHeader()))
                         .bind("var"),
                     this);
}

void StaticStorageClassCheck::check(const MatchFinder::MatchResult &Result) {
  // Check function declarations
  if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
      return;

    // Skip main function
    if (FD->isMain())
      return;

    // Skip implicit functions (compiler-generated)
    if (FD->isImplicit())
      return;

    // Skip functions with external linkage
    if (FD->hasExternalFormalLinkage())
      return;

    // Check if this function has internal linkage but is missing static
    if (!FD->isStatic()) {
      // Find the first declaration to see if any declaration has static
      bool hasStaticInChain = false;
      for (const FunctionDecl *Redecl : FD->redecls()) {
        if (Redecl->isStatic()) {
          hasStaticInChain = true;
          break;
        }
      }

      // If any declaration in the chain has static, or if this has internal
      // linkage, it should have static
      if (hasStaticInChain || FD->getLinkageInternal() == Linkage::Internal) {
        diag(FD->getLocation(),
             "function '%0' has internal linkage but is missing the static "
             "storage class specifier")
            << FD->getName();

        // Add a note pointing to a declaration with static if one exists
        if (hasStaticInChain) {
          for (const FunctionDecl *Redecl : FD->redecls()) {
            if (Redecl->isStatic() && Redecl != FD) {
              diag(Redecl->getLocation(),
                   "previous declaration with static "
                   "storage class specifier is here",
                   DiagnosticIDs::Note);
              break;
            }
          }
        }
      }
    }
    return;
  }

  // Check variable declarations
  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;

    // Skip variables with external storage (extern keyword)
    if (VD->hasExternalStorage())
      return;

    // Skip variables with external linkage
    if (VD->hasExternalFormalLinkage())
      return;

    // Check if this variable has internal linkage but is missing static
    if (VD->getStorageClass() != SC_Static) {
      // Find if any declaration has static
      bool hasStaticInChain = false;
      for (const VarDecl *Redecl : VD->redecls()) {
        if (Redecl->getStorageClass() == SC_Static) {
          hasStaticInChain = true;
          break;
        }
      }

      // If any declaration in the chain has static, or if this has internal
      // linkage, it should have static
      if (hasStaticInChain || VD->getLinkageInternal() == Linkage::Internal) {
        diag(VD->getLocation(),
             "variable '%0' has internal linkage but is missing the static "
             "storage class specifier")
            << VD->getName();

        // Add a note pointing to a declaration with static if one exists
        if (hasStaticInChain) {
          for (const VarDecl *Redecl : VD->redecls()) {
            if (Redecl->getStorageClass() == SC_Static && Redecl != VD) {
              diag(Redecl->getLocation(),
                   "previous declaration with static "
                   "storage class specifier is here",
                   DiagnosticIDs::Note);
              break;
            }
          }
        }
      }
    }
  }
}

} // namespace clang::tidy::automotive

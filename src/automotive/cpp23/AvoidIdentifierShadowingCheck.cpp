//===--- AvoidIdentifierShadowingCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidIdentifierShadowingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclCXX.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidIdentifierShadowingCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations in local scopes
  Finder->addMatcher(
      varDecl(hasLocalStorage(), unless(parmVarDecl())).bind("var"), this);
}

void AvoidIdentifierShadowingCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!VD)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  // Skip unnamed or anonymous variables
  if (VD->getName().empty())
    return;

  // Skip implicit declarations
  if (VD->isImplicit())
    return;

  // Get the declaring DeclContext
  const DeclContext *DC = VD->getDeclContext();
  if (!DC)
    return;

  StringRef Name = VD->getName();

  // Walk up the enclosing scopes looking for shadowed declarations
  const DeclContext *Parent = DC->getParent();
  while (Parent) {
    // Check for shadowed variables in enclosing function scopes
    if (const auto *FD = dyn_cast<FunctionDecl>(Parent)) {
      // Check parameters
      for (const auto *Param : FD->parameters()) {
        if (Param->getName() == Name) {
          diag(VD->getLocation(),
               "local variable %0 shadows parameter in enclosing function")
              << VD;
          diag(Param->getLocation(), "parameter declared here",
               DiagnosticIDs::Note);
          return;
        }
      }
    }

    // Check for shadowing in enclosing blocks/compound statements
    if (const auto *BS = dyn_cast<BlockDecl>(Parent)) {
      for (const auto *D : BS->decls()) {
        if (const auto *ShadowedVar = dyn_cast<VarDecl>(D)) {
          if (ShadowedVar != VD && ShadowedVar->getName() == Name) {
            diag(VD->getLocation(),
                 "local variable %0 shadows variable in enclosing scope")
                << VD;
            diag(ShadowedVar->getLocation(), "variable declared here",
                 DiagnosticIDs::Note);
            return;
          }
        }
      }
    }

    // Check for shadowed class/struct members
    if (const auto *RD = dyn_cast<CXXRecordDecl>(Parent)) {
      for (const auto *Field : RD->fields()) {
        if (Field->getName() == Name) {
          diag(VD->getLocation(),
               "local variable %0 shadows member of enclosing class")
              << VD;
          diag(Field->getLocation(), "member declared here",
               DiagnosticIDs::Note);
          return;
        }
      }
    }

    // Check namespace-scope and file-scope variables
    if (isa<NamespaceDecl>(Parent) || isa<TranslationUnitDecl>(Parent)) {
      // Use lookup to find declarations
      for (const auto *D : Parent->decls()) {
        if (const auto *ShadowedVar = dyn_cast<VarDecl>(D)) {
          if (ShadowedVar->getName() == Name) {
            diag(VD->getLocation(),
                 "local variable %0 shadows global/namespace-scope variable")
                << VD;
            diag(ShadowedVar->getLocation(), "variable declared here",
                 DiagnosticIDs::Note);
            return;
          }
        }
      }
    }

    Parent = Parent->getParent();
  }
}

} // namespace clang::tidy::automotive

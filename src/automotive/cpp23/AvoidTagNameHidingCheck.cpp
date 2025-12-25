//===--- AvoidTagNameHidingCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTagNameHidingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclCXX.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidTagNameHidingCheck::registerMatchers(MatchFinder *Finder) {
  // Match all tag declarations (struct, class, enum, union)
  Finder->addMatcher(tagDecl(unless(isImplicit())).bind("tag"), this);
}

void AvoidTagNameHidingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *TD = Result.Nodes.getNodeAs<TagDecl>("tag");
  if (!TD)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(TD->getLocation()))
    return;

  // Skip unnamed or anonymous tags
  if (!TD->getIdentifier() || TD->getName().empty())
    return;

  // Skip implicit declarations
  if (TD->isImplicit())
    return;

  // Get the declaring DeclContext
  const DeclContext *DC = TD->getDeclContext();
  if (!DC)
    return;

  StringRef TagName = TD->getName();

  // Check current scope if it's a function (for parameters)
  if (const auto *FD = dyn_cast<FunctionDecl>(DC)) {
    for (const auto *Param : FD->parameters()) {
      if (Param->getName() == TagName) {
        diag(TD->getLocation(),
             "tag name %0 hides parameter in enclosing function")
            << TD;
        diag(Param->getLocation(), "parameter declared here",
             DiagnosticIDs::Note);
        return;
      }
    }
  }

  // Walk up the enclosing scopes looking for hidden declarations
  const DeclContext *Parent = DC->getParent();
  while (Parent) {
    // Check for hidden variables in function scopes
    if (const auto *FD = dyn_cast<FunctionDecl>(Parent)) {
      // Check parameters
      for (const auto *Param : FD->parameters()) {
        if (Param->getName() == TagName) {
          diag(TD->getLocation(),
               "tag name %0 hides parameter in enclosing function")
              << TD;
          diag(Param->getLocation(), "parameter declared here",
               DiagnosticIDs::Note);
          return;
        }
      }
    }

    // Check for hidden declarations in blocks
    if (const auto *BS = dyn_cast<BlockDecl>(Parent)) {
      for (const auto *D : BS->decls()) {
        if (const auto *VD = dyn_cast<VarDecl>(D)) {
          if (VD->getName() == TagName) {
            diag(TD->getLocation(),
                 "tag name %0 hides variable in enclosing scope")
                << TD;
            diag(VD->getLocation(), "variable declared here",
                 DiagnosticIDs::Note);
            return;
          }
        }
      }
    }

    // Check for hidden class/struct members
    if (const auto *RD = dyn_cast<CXXRecordDecl>(Parent)) {
      for (const auto *Field : RD->fields()) {
        if (Field->getName() == TagName) {
          diag(TD->getLocation(), "tag name %0 hides member of enclosing class")
              << TD;
          diag(Field->getLocation(), "member declared here",
               DiagnosticIDs::Note);
          return;
        }
      }
    }

    // Check namespace-scope and file-scope declarations
    if (isa<NamespaceDecl>(Parent) || isa<TranslationUnitDecl>(Parent)) {
      for (const auto *D : Parent->decls()) {
        // Check for variable declarations
        if (const auto *VD = dyn_cast<VarDecl>(D)) {
          if (VD->getName() == TagName) {
            diag(TD->getLocation(),
                 "tag name %0 hides global/namespace-scope variable")
                << TD;
            diag(VD->getLocation(), "variable declared here",
                 DiagnosticIDs::Note);
            return;
          }
        }
        // Check for function declarations
        if (const auto *FuncD = dyn_cast<FunctionDecl>(D)) {
          if (FuncD->getName() == TagName) {
            diag(TD->getLocation(),
                 "tag name %0 hides function in enclosing scope")
                << TD;
            diag(FuncD->getLocation(), "function declared here",
                 DiagnosticIDs::Note);
            return;
          }
        }
        // Check for typedef declarations
        if (const auto *TypedefD = dyn_cast<TypedefNameDecl>(D)) {
          if (TypedefD->getName() == TagName) {
            diag(TD->getLocation(),
                 "tag name %0 hides typedef in enclosing scope")
                << TD;
            diag(TypedefD->getLocation(), "typedef declared here",
                 DiagnosticIDs::Note);
            return;
          }
        }
        // Check for enum constants
        if (const auto *EnumD = dyn_cast<EnumDecl>(D)) {
          for (const auto *EC : EnumD->enumerators()) {
            if (EC->getName() == TagName) {
              diag(TD->getLocation(),
                   "tag name %0 hides enumerator in enclosing scope")
                  << TD;
              diag(EC->getLocation(), "enumerator declared here",
                   DiagnosticIDs::Note);
              return;
            }
          }
        }
      }
    }

    Parent = Parent->getParent();
  }
}

} // namespace clang::tidy::automotive::cpp23

//===--- UnconstrainedTemplateCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnconstrainedTemplateCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
// Helper visitor to check for static_assert in function bodies
class StaticAssertFinder : public RecursiveASTVisitor<StaticAssertFinder> {
public:
  bool Found = false;

  bool VisitStaticAssertDecl(StaticAssertDecl *) {
    Found = true;
    return false; // Stop traversal
  }
};
} // namespace

void UnconstrainedTemplateCheck::registerMatchers(MatchFinder *Finder) {
  // Match function templates
  Finder->addMatcher(functionTemplateDecl(unless(isExpansionInSystemHeader()))
                         .bind("funcTemplate"),
                     this);

  // Match class templates
  Finder->addMatcher(classTemplateDecl(unless(isExpansionInSystemHeader()))
                         .bind("classTemplate"),
                     this);
}

bool UnconstrainedTemplateCheck::hasConstraints(
    const FunctionTemplateDecl *FTD) const {
  // Check if template has requires clause
  if (const auto *FD = FTD->getTemplatedDecl()) {
    if (FD->getTrailingRequiresClause())
      return true;
  }

  // Check template parameters for constraints
  const auto *TPL = FTD->getTemplateParameters();
  if (!TPL)
    return false;

  for (const auto *Param : *TPL) {
    // Check for constrained type parameters (concepts)
    if (const auto *TTPD = dyn_cast<TemplateTypeParmDecl>(Param)) {
      if (TTPD->hasTypeConstraint())
        return true;
    }
    // Non-type template parameters are inherently constrained
    if (isa<NonTypeTemplateParmDecl>(Param))
      return true;
  }

  return false;
}

bool UnconstrainedTemplateCheck::hasConstraints(
    const ClassTemplateDecl *CTD) const {
  // Check template parameters for constraints
  const auto *TPL = CTD->getTemplateParameters();
  if (!TPL)
    return false;

  for (const auto *Param : *TPL) {
    // Check for constrained type parameters (concepts)
    if (const auto *TTPD = dyn_cast<TemplateTypeParmDecl>(Param)) {
      if (TTPD->hasTypeConstraint())
        return true;
    }
    // Non-type template parameters are inherently constrained
    if (isa<NonTypeTemplateParmDecl>(Param))
      return true;
  }

  // Check if the templated class has a requires clause
  if (const auto *RD = CTD->getTemplatedDecl()) {
    // Check for static_assert in the class body
    for (const auto *D : RD->decls()) {
      if (isa<StaticAssertDecl>(D))
        return true;
    }
  }

  return false;
}

bool UnconstrainedTemplateCheck::hasStaticAssert(const FunctionDecl *FD) const {
  if (!FD->hasBody())
    return false;

  StaticAssertFinder Finder;
  Finder.TraverseStmt(FD->getBody());
  return Finder.Found;
}

void UnconstrainedTemplateCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *FTD =
          Result.Nodes.getNodeAs<FunctionTemplateDecl>("funcTemplate")) {
    // Skip if template has constraints
    if (hasConstraints(FTD))
      return;

    // Skip if function body has static_assert
    if (const auto *FD = FTD->getTemplatedDecl()) {
      if (hasStaticAssert(FD))
        return;

      // Skip member functions of class templates (class already handles
      // constraints)
      if (FD->isCXXClassMember())
        return;

      // Skip operator overloads
      if (FD->isOverloadedOperator())
        return;
    }

    diag(FTD->getLocation(),
         "function template %0 has unconstrained type parameters; consider "
         "using concepts or static_assert")
        << FTD;
    return;
  }

  if (const auto *CTD =
          Result.Nodes.getNodeAs<ClassTemplateDecl>("classTemplate")) {
    // Skip if template has constraints
    if (hasConstraints(CTD))
      return;

    diag(CTD->getLocation(),
         "class template %0 has unconstrained type parameters; consider "
         "using concepts or static_assert")
        << CTD;
  }
}

} // namespace clang::tidy::automotive

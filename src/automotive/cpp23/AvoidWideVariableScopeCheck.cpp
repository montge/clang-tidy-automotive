//===--- AvoidWideVariableScopeCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidWideVariableScopeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {

/// Visitor to collect all references to a specific variable
class VarRefCollector : public RecursiveASTVisitor<VarRefCollector> {
public:
  VarRefCollector(const VarDecl *VD) : TargetVar(VD) {}

  bool VisitDeclRefExpr(DeclRefExpr *DRE) {
    if (DRE->getDecl() == TargetVar) {
      References.push_back(DRE);
    }
    return true;
  }

  const SmallVector<const DeclRefExpr *, 8> &getReferences() const {
    return References;
  }

private:
  const VarDecl *TargetVar;
  SmallVector<const DeclRefExpr *, 8> References;
};

/// Find the common ancestor compound statement for multiple statements
class CommonScopeFinder {
public:
  CommonScopeFinder(ASTContext *Context) : Context(Context) {}

  const CompoundStmt *findCommonScope(const Stmt *S1, const Stmt *S2) {
    // Get all ancestor compound statements for both statements
    SmallVector<const CompoundStmt *, 8> Ancestors1 = getCompoundAncestors(S1);
    SmallVector<const CompoundStmt *, 8> Ancestors2 = getCompoundAncestors(S2);

    // Find the deepest (last) common ancestor
    const CompoundStmt *Common = nullptr;
    size_t MinSize = std::min(Ancestors1.size(), Ancestors2.size());

    for (size_t i = 0; i < MinSize; ++i) {
      if (Ancestors1[Ancestors1.size() - 1 - i] ==
          Ancestors2[Ancestors2.size() - 1 - i]) {
        Common = Ancestors1[Ancestors1.size() - 1 - i];
      } else {
        break;
      }
    }

    return Common;
  }

  const CompoundStmt *
  findCommonScopeForAll(ArrayRef<const DeclRefExpr *> Refs) {
    if (Refs.empty())
      return nullptr;
    if (Refs.size() == 1)
      return findInnermostCompound(Refs[0]);

    const CompoundStmt *Common = findInnermostCompound(Refs[0]);
    for (size_t i = 1; i < Refs.size() && Common; ++i) {
      Common = findCommonScope(Common, Refs[i]);
    }
    return Common;
  }

private:
  ASTContext *Context;

  SmallVector<const CompoundStmt *, 8> getCompoundAncestors(const Stmt *S) {
    SmallVector<const CompoundStmt *, 8> Ancestors;
    const Stmt *Current = S;

    while (true) {
      DynTypedNodeList Parents =
          Context->getParentMapContext().getParents(*Current);
      if (Parents.empty())
        break;

      const Stmt *ParentStmt = Parents[0].get<Stmt>();
      if (!ParentStmt)
        break;

      if (const auto *CS = dyn_cast<CompoundStmt>(ParentStmt)) {
        Ancestors.push_back(CS);
      }

      Current = ParentStmt;
    }

    return Ancestors;
  }

  const CompoundStmt *findInnermostCompound(const Stmt *S) {
    const Stmt *Current = S;

    while (true) {
      DynTypedNodeList Parents =
          Context->getParentMapContext().getParents(*Current);
      if (Parents.empty())
        break;

      const Stmt *ParentStmt = Parents[0].get<Stmt>();
      if (!ParentStmt)
        break;

      if (const auto *CS = dyn_cast<CompoundStmt>(ParentStmt)) {
        return CS;
      }

      Current = ParentStmt;
    }

    return nullptr;
  }
};

} // anonymous namespace

void AvoidWideVariableScopeCheck::registerMatchers(MatchFinder *Finder) {
  // Match local variable declarations that are not parameters or static
  Finder->addMatcher(
      varDecl(hasLocalStorage(), unless(parmVarDecl()), unless(isStaticLocal()),
              unless(isImplicit()),
              hasParent(declStmt(hasParent(compoundStmt().bind("scope")))))
          .bind("var"),
      this);
}

void AvoidWideVariableScopeCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *DeclScope = Result.Nodes.getNodeAs<CompoundStmt>("scope");

  if (!VD || !DeclScope)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  // Skip unnamed variables
  if (VD->getName().empty())
    return;

  // Skip variables with initializers that have side effects or are complex
  if (const Expr *Init = VD->getInit()) {
    if (Init->HasSideEffects(*Result.Context))
      return;
  }

  // Collect all references to this variable
  VarRefCollector Collector(VD);
  Collector.TraverseStmt(const_cast<CompoundStmt *>(DeclScope));

  const auto &Refs = Collector.getReferences();

  // If variable is never used, let unused-variable check handle it
  if (Refs.empty())
    return;

  // Find the minimal scope that contains all uses
  CommonScopeFinder ScopeFinder(Result.Context);
  const CompoundStmt *MinimalScope = ScopeFinder.findCommonScopeForAll(Refs);

  if (!MinimalScope)
    return;

  // Check if the minimal scope is different from (and nested within) the
  // declaration scope
  if (MinimalScope != DeclScope) {
    // Verify that MinimalScope is actually nested within DeclScope
    const Stmt *Current = MinimalScope;
    bool IsNested = false;

    while (true) {
      DynTypedNodeList Parents =
          Result.Context->getParentMapContext().getParents(*Current);
      if (Parents.empty())
        break;

      const Stmt *ParentStmt = Parents[0].get<Stmt>();
      if (!ParentStmt)
        break;

      if (ParentStmt == DeclScope) {
        IsNested = true;
        break;
      }

      Current = ParentStmt;
    }

    if (IsNested) {
      // Find the location of the minimal scope for better diagnostics
      SourceLocation ScopeLocation = MinimalScope->getBeginLoc();

      diag(VD->getLocation(),
           "variable %0 is declared in a wider scope than necessary")
          << VD;
      diag(ScopeLocation, "variable could be declared in this narrower scope",
           DiagnosticIDs::Note);
    }
  }
}

void AvoidWideVariableScopeCheck::collectVarReferences(
    const Stmt *Root, const VarDecl *VD,
    SmallVectorImpl<const DeclRefExpr *> &Refs) const {
  if (!Root)
    return;

  if (const auto *DRE = dyn_cast<DeclRefExpr>(Root)) {
    if (DRE->getDecl() == VD) {
      Refs.push_back(DRE);
    }
  }

  for (const Stmt *Child : Root->children()) {
    collectVarReferences(Child, VD, Refs);
  }
}

const CompoundStmt *
AvoidWideVariableScopeCheck::findMinimalScope(const VarDecl *VD,
                                              const CompoundStmt *DeclScope,
                                              ASTContext *Context) const {
  SmallVector<const DeclRefExpr *, 8> Refs;
  collectVarReferences(DeclScope, VD, Refs);

  if (Refs.empty())
    return nullptr;

  // For now, return the declaration scope
  // A more sophisticated implementation would find the common ancestor
  return DeclScope;
}

bool AvoidWideVariableScopeCheck::isAncestor(const Stmt *Ancestor,
                                             const Stmt *Descendant,
                                             ASTContext *Context) const {
  if (!Ancestor || !Descendant)
    return false;

  const Stmt *Current = Descendant;
  while (Current) {
    if (Current == Ancestor)
      return true;

    DynTypedNodeList Parents =
        Context->getParentMapContext().getParents(*Current);
    if (Parents.empty())
      break;

    Current = Parents[0].get<Stmt>();
  }

  return false;
}

const CompoundStmt *AvoidWideVariableScopeCheck::findContainingCompoundStmt(
    const Stmt *S, const Stmt *Root) const {
  if (!S || !Root)
    return nullptr;

  // Simple traversal - a real implementation would use parent maps
  return nullptr;
}

} // namespace clang::tidy::automotive::cpp23

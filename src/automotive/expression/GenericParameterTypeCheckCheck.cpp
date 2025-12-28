//===--- GenericParameterTypeCheckCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericParameterTypeCheckCheck.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/SmallSet.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
// Helper visitor to collect DeclRefExpr nodes
class DeclRefCollector : public RecursiveASTVisitor<DeclRefCollector> {
public:
  llvm::SmallVectorImpl<const DeclRefExpr *> &DeclRefs;

  explicit DeclRefCollector(
      llvm::SmallVectorImpl<const DeclRefExpr *> &DeclRefs)
      : DeclRefs(DeclRefs) {}

  bool VisitDeclRefExpr(const DeclRefExpr *DRE) {
    DeclRefs.push_back(DRE);
    return true;
  }
};
} // namespace

void GenericParameterTypeCheckCheck::collectDeclRefs(
    const Expr *E, llvm::SmallVectorImpl<const DeclRefExpr *> &DeclRefs) {
  if (!E)
    return;
  DeclRefCollector Collector(DeclRefs);
  Collector.TraverseStmt(const_cast<Expr *>(E));
}

void GenericParameterTypeCheckCheck::registerMatchers(MatchFinder *Finder) {
  // Match generic selection expressions that come from macros
  // This is an advisory check focusing on macro usage
  Finder->addMatcher(
      genericSelectionExpr(isExpansionInMainFile()).bind("genericSelection"),
      this);
}

void GenericParameterTypeCheckCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  // Only check if this is from a macro expansion
  SourceLocation Loc = GenericSelection->getGenericLoc();
  if (!Loc.isMacroID())
    return;

  unsigned NumAssocs = GenericSelection->getNumAssocs();
  if (NumAssocs == 0)
    return;

  // Collect all parameters (DeclRefExprs) used in each association
  llvm::SmallVector<llvm::SmallSet<const ValueDecl *, 8>, 8> AssocParams;

  for (unsigned I = 0; I < NumAssocs; ++I) {
    auto Assoc = GenericSelection->getAssociation(I);
    const Expr *AssocExpr = Assoc.getAssociationExpr();

    llvm::SmallVector<const DeclRefExpr *, 8> DeclRefs;
    collectDeclRefs(AssocExpr, DeclRefs);

    llvm::SmallSet<const ValueDecl *, 8> Params;
    for (const auto *DRE : DeclRefs) {
      if (const auto *VD = dyn_cast<ValueDecl>(DRE->getDecl())) {
        // Only consider parameters (ParmVarDecl) or variables that might
        // represent macro parameters
        if (isa<ParmVarDecl>(VD) || isa<VarDecl>(VD)) {
          Params.insert(VD);
        }
      }
    }

    AssocParams.push_back(Params);
  }

  // Check if all associations use the same set of parameters
  if (AssocParams.size() < 2)
    return;

  // Build union of all parameters
  llvm::SmallSet<const ValueDecl *, 8> AllParams;
  for (const auto &Params : AssocParams) {
    for (const auto *Param : Params) {
      AllParams.insert(Param);
    }
  }

  // Check if each association uses all parameters
  for (unsigned I = 0; I < AssocParams.size(); ++I) {
    auto Assoc = GenericSelection->getAssociation(I);

    // Skip default association
    if (!Assoc.getTypeSourceInfo())
      continue;

    for (const auto *Param : AllParams) {
      if (AssocParams[I].count(Param) == 0) {
        // This association doesn't use this parameter
        diag(Assoc.getAssociationExpr()->getBeginLoc(),
             "generic parameter '%0' should be type-checked in all "
             "associations but is missing in this one")
            << Param->getName();
      }
    }
  }
}

} // namespace clang::tidy::automotive

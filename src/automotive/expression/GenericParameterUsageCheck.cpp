//===--- GenericParameterUsageCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericParameterUsageCheck.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
/// Helper visitor to count DeclRefExpr occurrences.
class DeclRefCounter : public RecursiveASTVisitor<DeclRefCounter> {
public:
  std::map<const ValueDecl *, unsigned> &Counts;

  explicit DeclRefCounter(std::map<const ValueDecl *, unsigned> &Counts)
      : Counts(Counts) {}

  bool VisitDeclRefExpr(const DeclRefExpr *DRE) {
    if (const auto *VD = dyn_cast<ValueDecl>(DRE->getDecl())) {
      Counts[VD]++;
    }
    return true;
  }
};
} // namespace

void GenericParameterUsageCheck::countDeclRefs(
    const Expr *E, std::map<const ValueDecl *, unsigned> &Counts) {
  if (!E)
    return;
  DeclRefCounter Counter(Counts);
  Counter.TraverseStmt(const_cast<Expr *>(E));
}

void GenericParameterUsageCheck::registerMatchers(MatchFinder *Finder) {
  // Match all generic selection expressions
  Finder->addMatcher(genericSelectionExpr().bind("genericSelection"), this);
}

void GenericParameterUsageCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  // Check controlling expression for multiple uses
  const Expr *ControllingExpr = GenericSelection->getControllingExpr();
  if (ControllingExpr) {
    std::map<const ValueDecl *, unsigned> ControllingCounts;
    countDeclRefs(ControllingExpr, ControllingCounts);

    for (const auto &Entry : ControllingCounts) {
      if (Entry.second > 1) {
        diag(ControllingExpr->getBeginLoc(),
             "generic selection controlling expression uses parameter '%0' "
             "%1 times; parameters should be used at most once")
            << Entry.first->getName() << Entry.second;
      }
    }
  }

  // Check each association expression for multiple uses
  unsigned NumAssocs = GenericSelection->getNumAssocs();
  for (unsigned I = 0; I < NumAssocs; ++I) {
    auto Assoc = GenericSelection->getAssociation(I);
    const Expr *AssocExpr = Assoc.getAssociationExpr();

    if (!AssocExpr)
      continue;

    std::map<const ValueDecl *, unsigned> AssocCounts;
    countDeclRefs(AssocExpr, AssocCounts);

    for (const auto &Entry : AssocCounts) {
      if (Entry.second > 1) {
        diag(AssocExpr->getBeginLoc(),
             "generic association expression uses parameter '%0' %1 times; "
             "parameters should be used at most once")
            << Entry.first->getName() << Entry.second;
      }
    }
  }
}

} // namespace clang::tidy::automotive

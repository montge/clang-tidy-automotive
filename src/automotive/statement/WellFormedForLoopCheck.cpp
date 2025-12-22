//===--- WellFormedForLoopCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "WellFormedForLoopCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Collect all VarDecls initialized in the for loop init statement
class InitVarCollector : public RecursiveASTVisitor<InitVarCollector> {
public:
  bool VisitVarDecl(VarDecl *VD) {
    if (VD->hasInit()) {
      InitVars.insert(VD);
    }
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (BO->isAssignmentOp()) {
      if (auto *DRE =
              dyn_cast<DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
        if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
          InitVars.insert(VD);
        }
      }
    }
    return true;
  }

  llvm::SmallPtrSet<const VarDecl *, 4> InitVars;
};

/// Check if a variable is referenced in an expression
class VarRefChecker : public RecursiveASTVisitor<VarRefChecker> {
public:
  explicit VarRefChecker(const llvm::SmallPtrSet<const VarDecl *, 4> &Vars)
      : TargetVars(Vars) {}

  bool VisitDeclRefExpr(DeclRefExpr *DRE) {
    if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
      if (TargetVars.count(VD)) {
        Found = true;
      }
    }
    return true;
  }

  bool isFound() const { return Found; }

private:
  const llvm::SmallPtrSet<const VarDecl *, 4> &TargetVars;
  bool Found = false;
};

/// Check if a variable is modified in an expression/statement
class ModificationChecker : public RecursiveASTVisitor<ModificationChecker> {
public:
  explicit ModificationChecker(
      const llvm::SmallPtrSet<const VarDecl *, 4> &Vars)
      : TargetVars(Vars) {}

  bool VisitUnaryOperator(UnaryOperator *UO) {
    if (UO->isIncrementDecrementOp()) {
      if (auto *DRE =
              dyn_cast<DeclRefExpr>(UO->getSubExpr()->IgnoreParenImpCasts())) {
        if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
          if (TargetVars.count(VD)) {
            Modified = true;
            ModificationLoc = UO->getOperatorLoc();
          }
        }
      }
    }
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (BO->isAssignmentOp()) {
      if (auto *DRE =
              dyn_cast<DeclRefExpr>(BO->getLHS()->IgnoreParenImpCasts())) {
        if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
          if (TargetVars.count(VD)) {
            Modified = true;
            ModificationLoc = BO->getOperatorLoc();
          }
        }
      }
    }
    return true;
  }

  // Don't traverse into nested loops - their modifications are separate
  bool TraverseForStmt(ForStmt *) { return true; }
  bool TraverseWhileStmt(WhileStmt *) { return true; }
  bool TraverseDoStmt(DoStmt *) { return true; }

  bool isModified() const { return Modified; }
  SourceLocation getModificationLoc() const { return ModificationLoc; }

private:
  const llvm::SmallPtrSet<const VarDecl *, 4> &TargetVars;
  bool Modified = false;
  SourceLocation ModificationLoc;
};

} // anonymous namespace

void WellFormedForLoopCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(forStmt().bind("for"), this);
}

void WellFormedForLoopCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *For = Result.Nodes.getNodeAs<ForStmt>("for");
  if (!For)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(For->getForLoc()))
    return;

  const Stmt *Init = For->getInit();
  const Expr *Cond = For->getCond();
  const Expr *Inc = For->getInc();
  const Stmt *Body = For->getBody();

  // Check 1: Init must exist and declare/initialize a loop counter
  if (!Init) {
    diag(For->getForLoc(),
         "for loop is missing initialization; "
         "a well-formed for loop should initialize a loop counter");
    return;
  }

  // Collect variables initialized in the init statement
  InitVarCollector Collector;
  // NOSONAR: const_cast required by RecursiveASTVisitor API
  Collector.TraverseStmt(const_cast<Stmt *>(Init));

  if (Collector.InitVars.empty()) {
    diag(For->getForLoc(),
         "for loop init statement does not initialize a loop counter");
    return;
  }

  // Check 2: Condition must exist and reference the loop counter
  if (!Cond) {
    diag(For->getForLoc(),
         "for loop is missing condition; "
         "a well-formed for loop should test the loop counter");
    return;
  }

  VarRefChecker CondChecker(Collector.InitVars);
  // NOSONAR: const_cast required by RecursiveASTVisitor API
  CondChecker.TraverseStmt(const_cast<Expr *>(Cond));

  if (!CondChecker.isFound()) {
    diag(Cond->getExprLoc(),
         "for loop condition does not reference the loop counter");
  }

  // Check 3: Increment must exist and modify the loop counter
  if (!Inc) {
    diag(For->getForLoc(),
         "for loop is missing increment expression; "
         "a well-formed for loop should modify the loop counter in the "
         "increment expression");
    return;
  }

  ModificationChecker IncChecker(Collector.InitVars);
  // NOSONAR: const_cast required by RecursiveASTVisitor API
  IncChecker.TraverseStmt(const_cast<Expr *>(Inc));

  if (!IncChecker.isModified()) {
    diag(Inc->getExprLoc(),
         "for loop increment expression does not modify the loop counter");
  }

  // Check 4: Loop counter should not be modified in the body
  if (Body) {
    ModificationChecker BodyChecker(Collector.InitVars);
    // NOSONAR: const_cast required by RecursiveASTVisitor API
    BodyChecker.TraverseStmt(const_cast<Stmt *>(Body));

    if (BodyChecker.isModified()) {
      diag(BodyChecker.getModificationLoc(),
           "loop counter modified in for loop body; "
           "the loop counter should only be modified in the increment "
           "expression");
    }
  }
}

} // namespace clang::tidy::automotive

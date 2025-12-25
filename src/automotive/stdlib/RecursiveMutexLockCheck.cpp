//===--- RecursiveMutexLockCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RecursiveMutexLockCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Visitor to analyze mutex lock/unlock patterns within a function
class MutexLockVisitor : public RecursiveASTVisitor<MutexLockVisitor> {
public:
  MutexLockVisitor(ClangTidyCheck &Check)
      : Check(Check) {}

  bool VisitCallExpr(CallExpr *Call) {
    const FunctionDecl *Callee = Call->getDirectCallee();
    if (!Callee)
      return true;

    StringRef Name = Callee->getName();

    // Check for mutex lock functions
    if (Name == "mtx_lock" || Name == "pthread_mutex_lock" ||
        Name == "mtx_timedlock" || Name == "pthread_mutex_timedlock" ||
        Name == "mtx_trylock" || Name == "pthread_mutex_trylock") {
      handleLock(Call);
    }
    // Check for mutex unlock functions
    else if (Name == "mtx_unlock" || Name == "pthread_mutex_unlock") {
      handleUnlock(Call);
    }

    return true;
  }

private:
  void handleLock(CallExpr *Call) {
    if (Call->getNumArgs() < 1)
      return;

    std::string MutexName = getMutexName(Call->getArg(0));
    if (MutexName.empty())
      return;

    // Check if this mutex is already locked
    if (LockedMutexes.count(MutexName)) {
      Check.diag(Call->getBeginLoc(),
                 "non-recursive mutex '%0' is being locked recursively; "
                 "this may cause deadlock")
          << MutexName;
      Check.diag(LockedMutexes[MutexName], "previous lock was here",
                 DiagnosticIDs::Note);
    } else {
      LockedMutexes[MutexName] = Call->getBeginLoc();
    }
  }

  void handleUnlock(CallExpr *Call) {
    if (Call->getNumArgs() < 1)
      return;

    std::string MutexName = getMutexName(Call->getArg(0));
    if (MutexName.empty())
      return;

    // Remove from locked set
    LockedMutexes.erase(MutexName);
  }

  std::string getMutexName(const Expr *E) {
    E = E->IgnoreParenImpCasts();

    // Handle &mutex
    if (const auto *UO = dyn_cast<UnaryOperator>(E)) {
      if (UO->getOpcode() == UO_AddrOf) {
        E = UO->getSubExpr()->IgnoreParenImpCasts();
      }
    }

    // Get the declaration reference
    if (const auto *DRE = dyn_cast<DeclRefExpr>(E)) {
      return DRE->getDecl()->getNameAsString();
    }

    // Handle member expressions (obj.mutex or obj->mutex)
    if (const auto *ME = dyn_cast<MemberExpr>(E)) {
      return ME->getMemberDecl()->getNameAsString();
    }

    return "";
  }

  ClangTidyCheck &Check;
  std::map<std::string, SourceLocation> LockedMutexes;
};

} // namespace

void RecursiveMutexLockCheck::registerMatchers(MatchFinder *Finder) {
  // Match function definitions to analyze their body
  Finder->addMatcher(functionDecl(isDefinition(), hasBody(stmt()))
                         .bind("function"),
                     this);
}

void RecursiveMutexLockCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("function");
  if (!Func || !Func->hasBody())
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Func->getLocation()))
    return;

  // Visit the function body to find lock patterns
  MutexLockVisitor Visitor(*this);
  Visitor.TraverseStmt(Func->getBody());
}

} // namespace clang::tidy::automotive

//===--- ThreadJoinDetachCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ThreadJoinDetachCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

enum class ThreadState { Created, Joined, Detached };

class ThreadLifecycleVisitor : public RecursiveASTVisitor<ThreadLifecycleVisitor> {
public:
  ThreadLifecycleVisitor(ClangTidyCheck &Check) : Check(Check) {}

  bool VisitCallExpr(CallExpr *Call) {
    const FunctionDecl *Callee = Call->getDirectCallee();
    if (!Callee)
      return true;

    StringRef Name = Callee->getName();

    // Thread creation
    if (Name == "thrd_create" || Name == "pthread_create") {
      handleCreate(Call);
    }
    // Thread join
    else if (Name == "thrd_join" || Name == "pthread_join") {
      handleJoin(Call);
    }
    // Thread detach
    else if (Name == "thrd_detach" || Name == "pthread_detach") {
      handleDetach(Call);
    }

    return true;
  }

private:
  void handleCreate(CallExpr *Call) {
    if (Call->getNumArgs() < 1)
      return;

    std::string ThreadName = getThreadName(Call->getArg(0));
    if (ThreadName.empty())
      return;

    ThreadStates[ThreadName] = {ThreadState::Created, Call->getBeginLoc()};
  }

  void handleJoin(CallExpr *Call) {
    if (Call->getNumArgs() < 1)
      return;

    std::string ThreadName = getThreadName(Call->getArg(0));
    if (ThreadName.empty())
      return;

    auto It = ThreadStates.find(ThreadName);
    if (It != ThreadStates.end()) {
      if (It->second.State == ThreadState::Joined) {
        Check.diag(Call->getBeginLoc(),
                   "thread '%0' has already been joined; "
                   "joining again causes undefined behavior")
            << ThreadName;
        Check.diag(It->second.Loc, "previous join was here",
                   DiagnosticIDs::Note);
      } else if (It->second.State == ThreadState::Detached) {
        Check.diag(Call->getBeginLoc(),
                   "thread '%0' was detached; "
                   "joining a detached thread causes undefined behavior")
            << ThreadName;
        Check.diag(It->second.Loc, "detach was here", DiagnosticIDs::Note);
      }
    }

    ThreadStates[ThreadName] = {ThreadState::Joined, Call->getBeginLoc()};
  }

  void handleDetach(CallExpr *Call) {
    if (Call->getNumArgs() < 1)
      return;

    std::string ThreadName = getThreadName(Call->getArg(0));
    if (ThreadName.empty())
      return;

    auto It = ThreadStates.find(ThreadName);
    if (It != ThreadStates.end()) {
      if (It->second.State == ThreadState::Detached) {
        Check.diag(Call->getBeginLoc(),
                   "thread '%0' has already been detached; "
                   "detaching again causes undefined behavior")
            << ThreadName;
        Check.diag(It->second.Loc, "previous detach was here",
                   DiagnosticIDs::Note);
      } else if (It->second.State == ThreadState::Joined) {
        Check.diag(Call->getBeginLoc(),
                   "thread '%0' was joined; "
                   "detaching a joined thread causes undefined behavior")
            << ThreadName;
        Check.diag(It->second.Loc, "join was here", DiagnosticIDs::Note);
      }
    }

    ThreadStates[ThreadName] = {ThreadState::Detached, Call->getBeginLoc()};
  }

  std::string getThreadName(const Expr *E) {
    E = E->IgnoreParenImpCasts();

    // Handle &thread
    if (const auto *UO = dyn_cast<UnaryOperator>(E)) {
      if (UO->getOpcode() == UO_AddrOf) {
        E = UO->getSubExpr()->IgnoreParenImpCasts();
      }
    }

    if (const auto *DRE = dyn_cast<DeclRefExpr>(E)) {
      return DRE->getDecl()->getNameAsString();
    }

    if (const auto *ME = dyn_cast<MemberExpr>(E)) {
      return ME->getMemberDecl()->getNameAsString();
    }

    return "";
  }

  struct StateInfo {
    ThreadState State;
    SourceLocation Loc;
  };

  ClangTidyCheck &Check;
  std::map<std::string, StateInfo> ThreadStates;
};

} // namespace

void ThreadJoinDetachCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(
      functionDecl(isDefinition(), hasBody(stmt())).bind("function"), this);
}

void ThreadJoinDetachCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("function");
  if (!Func || !Func->hasBody())
    return;

  if (Result.SourceManager->isInSystemHeader(Func->getLocation()))
    return;

  ThreadLifecycleVisitor Visitor(*this);
  Visitor.TraverseStmt(Func->getBody());
}

} // namespace clang::tidy::automotive

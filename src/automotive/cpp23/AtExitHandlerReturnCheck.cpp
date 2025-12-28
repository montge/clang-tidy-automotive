//===--- AtExitHandlerReturnCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AtExitHandlerReturnCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {

class AtExitFunctionVisitor : public RecursiveASTVisitor<AtExitFunctionVisitor> {
public:
  explicit AtExitFunctionVisitor(AtExitHandlerReturnCheck &Check,
                                  ASTContext &Context)
      : Check(Check), Context(Context) {}

  bool VisitCXXThrowExpr(CXXThrowExpr *Throw) {
    Check.diag(Throw->getThrowLoc(),
               "at-exit handler shall not throw exceptions; must terminate by "
               "returning (MISRA C++:2023 Rule 10.2)");
    return true;
  }

  bool VisitCallExpr(CallExpr *Call) {
    if (const FunctionDecl *Callee = Call->getDirectCallee()) {
      StringRef Name = Callee->getName();
      if (Name == "exit" || Name == "abort" || Name == "_Exit" ||
          Name == "quick_exit" || Name == "longjmp") {
        Check.diag(Call->getBeginLoc(),
                   "at-exit handler shall not call %0; must terminate by "
                   "returning (MISRA C++:2023 Rule 10.2)")
            << Name;
      }
    }
    return true;
  }

private:
  AtExitHandlerReturnCheck &Check;
  ASTContext &Context;
};

} // namespace

void AtExitHandlerReturnCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to atexit, at_quick_exit, and similar functions
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("atexit", "at_quick_exit",
                                               "std::atexit", "std::at_quick_exit"))))
          .bind("atexit_call"),
      this);
}

void AtExitHandlerReturnCheck::check(
    const MatchFinder::MatchResult &Result) {

  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("atexit_call");
  if (!Call || Call->getNumArgs() < 1)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  // Get the function argument (the at-exit handler)
  const Expr *Arg = Call->getArg(0)->IgnoreParenImpCasts();

  const FunctionDecl *HandlerFunc = nullptr;

  // Try to extract the function from various forms
  if (const auto *DRE = dyn_cast<DeclRefExpr>(Arg)) {
    HandlerFunc = dyn_cast<FunctionDecl>(DRE->getDecl());
  } else if (const auto *UO = dyn_cast<UnaryOperator>(Arg)) {
    if (UO->getOpcode() == UO_AddrOf) {
      if (const auto *DRE = dyn_cast<DeclRefExpr>(UO->getSubExpr())) {
        HandlerFunc = dyn_cast<FunctionDecl>(DRE->getDecl());
      }
    }
  }

  if (HandlerFunc && HandlerFunc->hasBody()) {
    checkAtExitFunction(HandlerFunc, Result.Context);
  }
}

void AtExitHandlerReturnCheck::checkAtExitFunction(const FunctionDecl *FD,
                                                     ASTContext *Context) {
  const Stmt *Body = FD->getBody();
  if (!Body)
    return;

  AtExitFunctionVisitor Visitor(*this, *Context);
  Visitor.TraverseStmt(const_cast<Stmt *>(Body));
}

} // namespace clang::tidy::automotive::cpp23

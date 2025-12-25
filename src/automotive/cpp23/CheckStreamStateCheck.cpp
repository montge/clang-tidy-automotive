//===--- CheckStreamStateCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CheckStreamStateCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/SmallSet.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {

// Check if a type is a standard stream type
bool isStreamType(QualType Type) {
  const auto *RT =
      Type->getUnqualifiedDesugaredType()->getAs<clang::RecordType>();
  if (!RT)
    return false;

  const auto *RD = RT->getDecl();
  if (!RD)
    return false;

  StringRef Name = RD->getName();
  return Name == "basic_istream" || Name == "basic_ostream" ||
         Name == "basic_iostream" || Name == "istream" || Name == "ostream" ||
         Name == "iostream" || Name == "ifstream" || Name == "ofstream" ||
         Name == "fstream" || Name == "istringstream" ||
         Name == "ostringstream" || Name == "stringstream";
}

// Check if an expression is a stream state check call
bool isStreamStateCheck(const Expr *E) {
  if (!E)
    return false;

  E = E->IgnoreParenImpCasts();

  // Check for calls to good(), fail(), eof(), bad(), operator bool()
  if (const auto *Call = dyn_cast<CXXMemberCallExpr>(E)) {
    if (const auto *Method = Call->getMethodDecl()) {
      StringRef MethodName = Method->getName();
      return MethodName == "good" || MethodName == "fail" ||
             MethodName == "eof" || MethodName == "bad";
    }
  }

  // Check for implicit bool conversion (if (stream))
  if (const auto *Cast = dyn_cast<CXXMemberCallExpr>(E)) {
    if (const auto *Method = Cast->getMethodDecl()) {
      if (Method->getNameAsString() == "operator bool" ||
          Method->getNameAsString() == "operator!") {
        return true;
      }
    }
  }

  return false;
}

// Check if statement contains a stream state check
bool hasStreamStateCheck(const Stmt *S, const VarDecl *StreamVar) {
  if (!S)
    return false;

  // Check if this is a condition that checks the stream
  if (const auto *IS = dyn_cast<IfStmt>(S)) {
    const Expr *Cond = IS->getCond();
    if (Cond) {
      Cond = Cond->IgnoreParenImpCasts();

      // Check for direct stream state check
      if (isStreamStateCheck(Cond))
        return true;

      // Check for stream bool conversion
      if (const auto *DRE = dyn_cast<DeclRefExpr>(Cond)) {
        if (DRE->getDecl() == StreamVar)
          return true;
      }

      // Check for member call on stream
      if (const auto *Call = dyn_cast<CXXMemberCallExpr>(Cond)) {
        if (const auto *Base = dyn_cast<DeclRefExpr>(
                Call->getImplicitObjectArgument()->IgnoreParenImpCasts())) {
          if (Base->getDecl() == StreamVar)
            return true;
        }
      }
    }
  }

  return false;
}

} // namespace

void CheckStreamStateCheck::registerMatchers(MatchFinder *Finder) {
  // Match chained stream operations (e.g., cin >> x >> y)
  Finder->addMatcher(
      cxxOperatorCallExpr(
          hasOverloadedOperatorName(">>"),
          hasArgument(0, cxxOperatorCallExpr(hasOverloadedOperatorName(">>"))
                             .bind("inner")))
          .bind("outer"),
      this);

  Finder->addMatcher(
      cxxOperatorCallExpr(
          hasOverloadedOperatorName("<<"),
          hasArgument(0, cxxOperatorCallExpr(hasOverloadedOperatorName("<<"))
                             .bind("inner")))
          .bind("outer"),
      this);

  // Match consecutive stream operations on same variable
  Finder->addMatcher(
      cxxOperatorCallExpr(
          anyOf(hasOverloadedOperatorName(">>"),
                hasOverloadedOperatorName("<<")),
          hasArgument(0, declRefExpr(to(varDecl().bind("streamVar")))))
          .bind("streamOp"),
      this);

  // Match getline calls
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("getline")))).bind("getline"), this);
}

void CheckStreamStateCheck::check(const MatchFinder::MatchResult &Result) {
  const SourceManager &SM = *Result.SourceManager;

  // Handle chained stream operations
  if (const auto *Outer =
          Result.Nodes.getNodeAs<CXXOperatorCallExpr>("outer")) {
    if (SM.isInSystemHeader(Outer->getBeginLoc()))
      return;

    diag(Outer->getOperatorLoc(),
         "chained stream operations without state checking; stream state "
         "shall be checked before use (MISRA C++:2023 Rule 30.0)");
    return;
  }

  // Handle consecutive stream operations
  if (const auto *StreamOp =
          Result.Nodes.getNodeAs<CXXOperatorCallExpr>("streamOp")) {
    if (SM.isInSystemHeader(StreamOp->getBeginLoc()))
      return;

    const auto *StreamVar = Result.Nodes.getNodeAs<VarDecl>("streamVar");
    if (!StreamVar)
      return;

    // Check if the stream type is a standard stream
    if (!isStreamType(StreamVar->getType()))
      return;

    // Get the parent statement to analyze context
    const auto &Parents = Result.Context->getParents(*StreamOp);
    if (Parents.empty())
      return;

    // Look for the next statement that uses the same stream
    const Stmt *CurrentStmt = StreamOp;
    for (const auto &Parent : Result.Context->getParents(*CurrentStmt)) {
      if (const auto *CS = Parent.get<CompoundStmt>()) {
        bool foundCurrentStmt = false;
        const Stmt *NextStmt = nullptr;

        for (const auto *Child : CS->body()) {
          if (foundCurrentStmt) {
            NextStmt = Child;
            break;
          }
          if (Child == CurrentStmt ||
              (CurrentStmt->getBeginLoc() >= Child->getBeginLoc() &&
               CurrentStmt->getEndLoc() <= Child->getEndLoc())) {
            foundCurrentStmt = true;
          }
        }

        // Check if next statement is a stream state check
        if (NextStmt && !hasStreamStateCheck(NextStmt, StreamVar)) {
          // Check if next statement uses the stream
          // This is a simplified heuristic - could be enhanced
          if (const auto *ExprStmt = dyn_cast<Expr>(NextStmt)) {
            const Expr *E = ExprStmt->IgnoreParenImpCasts();
            if (const auto *OpCall = dyn_cast<CXXOperatorCallExpr>(E)) {
              if (OpCall->getNumArgs() > 0) {
                if (const auto *DRE = dyn_cast<DeclRefExpr>(
                        OpCall->getArg(0)->IgnoreParenImpCasts())) {
                  if (DRE->getDecl() == StreamVar) {
                    diag(OpCall->getOperatorLoc(),
                         "stream operation without prior state check; stream "
                         "state shall be checked before use (MISRA C++:2023 "
                         "Rule 30.0)");
                  }
                }
              }
            }
          }
        }
        break;
      }
    }
  }

  // Handle getline calls
  if (const auto *Getline = Result.Nodes.getNodeAs<CallExpr>("getline")) {
    if (SM.isInSystemHeader(Getline->getBeginLoc()))
      return;

    // Get the parent to see if result is checked
    const auto &Parents = Result.Context->getParents(*Getline);
    if (!Parents.empty()) {
      bool InCondition = false;

      // Walk up the parent chain to see if we're in a condition
      const Expr *CurrentExpr = Getline;
      for (size_t i = 0; i < 5 && CurrentExpr; ++i) {
        auto CurParents = Result.Context->getParents(*CurrentExpr);
        if (CurParents.empty())
          break;

        const Stmt *Parent = CurParents[0].get<Stmt>();
        if (!Parent)
          break;

        // Check if this parent is an IfStmt or WhileStmt with CurrentExpr as
        // condition
        if (const auto *IS = dyn_cast<clang::IfStmt>(Parent)) {
          if (IS->getCond() &&
              IS->getCond()->getBeginLoc() <= CurrentExpr->getBeginLoc() &&
              IS->getCond()->getEndLoc() >= CurrentExpr->getEndLoc()) {
            InCondition = true;
            break;
          }
        }
        if (const auto *WS = dyn_cast<clang::WhileStmt>(Parent)) {
          if (WS->getCond() &&
              WS->getCond()->getBeginLoc() <= CurrentExpr->getBeginLoc() &&
              WS->getCond()->getEndLoc() >= CurrentExpr->getEndLoc()) {
            InCondition = true;
            break;
          }
        }

        CurrentExpr = dyn_cast<Expr>(Parent);
      }

      // If not in a condition, warn
      if (!InCondition) {
        diag(Getline->getBeginLoc(),
             "getline() result not checked; stream state shall be checked "
             "after use (MISRA C++:2023 Rule 30.0)");
      }
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

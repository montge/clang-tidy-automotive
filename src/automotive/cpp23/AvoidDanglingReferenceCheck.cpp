//===--- AvoidDanglingReferenceCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidDanglingReferenceCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidDanglingReferenceCheck::registerMatchers(MatchFinder *Finder) {
  // Match return statements that return address of local variable (&local)
  Finder->addMatcher(
      returnStmt(
          hasReturnValue(ignoringImpCasts(unaryOperator(
              hasOperatorName("&"),
              hasUnaryOperand(declRefExpr(to(
                  varDecl(hasAutomaticStorageDuration()).bind("addr_var"))))))))
          .bind("return_addr"),
      this);

  // Match return statements that return reference to local variable
  // This requires checking if the function returns a reference type
  Finder->addMatcher(
      returnStmt(hasReturnValue(ignoringImpCasts(declRefExpr(to(
                     varDecl(hasAutomaticStorageDuration()).bind("ref_var"))))))
          .bind("return_ref"),
      this);

  // Match return statements that return pointer to local variable
  // (direct pointer assignment)
  Finder->addMatcher(
      returnStmt(
          hasReturnValue(ignoringImpCasts(declRefExpr(
              to(varDecl(hasAutomaticStorageDuration(), hasType(pointerType()))
                     .bind("ptr_var"))))))
          .bind("return_ptr"),
      this);

  // Match return of pointer-typed expression that evaluates to address of local
  Finder->addMatcher(
      returnStmt(hasReturnValue(
                     implicitCastExpr(hasCastKind(CK_LValueToRValue),
                                      hasSourceExpression(declRefExpr(to(
                                          varDecl(hasAutomaticStorageDuration())
                                              .bind("implicit_var")))))
                         .bind("implicit_cast")))
          .bind("return_implicit"),
      this);

  // Match lambda expressions that capture local variables by reference
  // and return them
  Finder->addMatcher(
      lambdaExpr(has(compoundStmt(has(returnStmt(hasReturnValue(
                     declRefExpr(to(varDecl().bind("lambda_capture_var")))
                         .bind("lambda_return")))))),
                 has(lambdaCapture().bind("capture")))
          .bind("lambda"),
      this);
}

bool AvoidDanglingReferenceCheck::isLocalAutoVariable(const VarDecl *VD) const {
  if (!VD)
    return false;

  // Check if it has automatic storage duration
  if (!VD->hasLocalStorage())
    return false;

  // Exclude static local variables
  if (VD->isStaticLocal())
    return false;

  // Exclude thread-local variables
  if (VD->getTSCSpec() != ThreadStorageClassSpecifier::TSCS_unspecified)
    return false;

  return true;
}

const VarDecl *
AvoidDanglingReferenceCheck::getReferencedLocalVar(const Expr *E) const {
  if (!E)
    return nullptr;

  E = E->IgnoreImpCasts();

  // Check for address-of operator
  if (const auto *UO = dyn_cast<UnaryOperator>(E)) {
    if (UO->getOpcode() == UO_AddrOf) {
      if (const auto *DRE =
              dyn_cast<DeclRefExpr>(UO->getSubExpr()->IgnoreImpCasts())) {
        if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
          if (isLocalAutoVariable(VD))
            return VD;
        }
      }
    }
  }

  // Check for direct reference
  if (const auto *DRE = dyn_cast<DeclRefExpr>(E)) {
    if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
      if (isLocalAutoVariable(VD))
        return VD;
    }
  }

  return nullptr;
}

void AvoidDanglingReferenceCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Check for return of address of local variable
  if (const auto *ReturnAddr =
          Result.Nodes.getNodeAs<ReturnStmt>("return_addr")) {
    if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("addr_var")) {
      // Skip system headers
      if (Result.SourceManager->isInSystemHeader(ReturnAddr->getBeginLoc()))
        return;

      diag(ReturnAddr->getReturnLoc(),
           "function returns address of local variable '%0' with automatic "
           "storage duration")
          << Var->getName();
      diag(Var->getLocation(), "variable declared here", DiagnosticIDs::Note);
    }
    return;
  }

  // Check for return of reference to local variable
  if (const auto *ReturnRef =
          Result.Nodes.getNodeAs<ReturnStmt>("return_ref")) {
    if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("ref_var")) {
      // Skip system headers
      if (Result.SourceManager->isInSystemHeader(ReturnRef->getBeginLoc()))
        return;

      // Get the function to check if it returns a reference or pointer
      const auto &Parents = Result.Context->getParents(*ReturnRef);
      if (!Parents.empty()) {
        const FunctionDecl *Func = nullptr;

        // Walk up to find the function
        auto CurrentParents = Parents;
        while (!CurrentParents.empty() && !Func) {
          Func = CurrentParents[0].get<FunctionDecl>();
          if (!Func && !CurrentParents.empty()) {
            CurrentParents = Result.Context->getParents(CurrentParents[0]);
          } else {
            break;
          }
        }

        if (Func) {
          QualType ReturnType = Func->getReturnType();

          // Check if function returns a reference type
          if (ReturnType->isReferenceType()) {
            diag(ReturnRef->getReturnLoc(),
                 "function returns reference to local variable '%0' with "
                 "automatic storage duration")
                << Var->getName();
            diag(Var->getLocation(), "variable declared here",
                 DiagnosticIDs::Note);
            return;
          }

          // Check if function returns a pointer type
          if (ReturnType->isPointerType()) {
            // Only warn if we're returning the address
            const Expr *RetValue = ReturnRef->getRetValue();
            if (RetValue) {
              if (const VarDecl *RefVar = getReferencedLocalVar(RetValue)) {
                diag(ReturnRef->getReturnLoc(),
                     "function returns pointer to local variable '%0' with "
                     "automatic storage duration")
                    << RefVar->getName();
                diag(RefVar->getLocation(), "variable declared here",
                     DiagnosticIDs::Note);
                return;
              }
            }
          }
        }
      }
    }
    return;
  }

  // Check for return of pointer to local variable
  if (const auto *ReturnPtr =
          Result.Nodes.getNodeAs<ReturnStmt>("return_ptr")) {
    if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("ptr_var")) {
      // Skip system headers
      if (Result.SourceManager->isInSystemHeader(ReturnPtr->getBeginLoc()))
        return;

      // Only warn if the pointer points to a local variable
      // This case handles: int *p = &local; return p;
      if (Var->hasInit()) {
        if (const VarDecl *InitVar = getReferencedLocalVar(Var->getInit())) {
          diag(ReturnPtr->getReturnLoc(),
               "function returns pointer variable '%0' that refers to local "
               "variable '%1' with automatic storage duration")
              << Var->getName() << InitVar->getName();
          diag(InitVar->getLocation(), "referenced variable declared here",
               DiagnosticIDs::Note);
          diag(Var->getLocation(), "pointer variable declared here",
               DiagnosticIDs::Note);
        }
      }
    }
    return;
  }

  // Check for implicit cast return cases
  // This case is mostly handled by the other matchers, so we skip it
  // to avoid redundant warnings
  if (Result.Nodes.getNodeAs<ReturnStmt>("return_implicit")) {
    return;
  }

  // Check for lambda returning captured local variables
  if (const auto *Lambda = Result.Nodes.getNodeAs<LambdaExpr>("lambda")) {
    if (const auto *Var =
            Result.Nodes.getNodeAs<VarDecl>("lambda_capture_var")) {
      if (const auto *ReturnExpr =
              Result.Nodes.getNodeAs<DeclRefExpr>("lambda_return")) {

        // Skip system headers
        if (Result.SourceManager->isInSystemHeader(Lambda->getBeginLoc()))
          return;

        // Check if the variable is captured by reference
        for (const LambdaCapture &Capture : Lambda->captures()) {
          if (Capture.capturesVariable() && Capture.getCapturedVar() == Var) {
            if (Capture.getCaptureKind() == LCK_ByRef) {
              // Check if the captured variable is local to the enclosing scope
              if (isLocalAutoVariable(Var)) {
                diag(ReturnExpr->getLocation(),
                     "lambda returns reference to local variable '%0' "
                     "captured by reference")
                    << Var->getName();
                diag(Var->getLocation(), "variable declared here",
                     DiagnosticIDs::Note);
              }
            }
          }
        }
      }
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

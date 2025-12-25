//===--- AvoidReturnLocalAddressCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidReturnLocalAddressCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidReturnLocalAddressCheck::registerMatchers(MatchFinder *Finder) {
  // Match return of address-of local variable
  Finder->addMatcher(
      returnStmt(hasReturnValue(unaryOperator(
                     hasOperatorName("&"),
                     hasUnaryOperand(declRefExpr(
                         to(varDecl(hasAutomaticStorageDuration(),
                                    unless(hasStaticStorageDuration()))
                                .bind("var")))))))
          .bind("return_addr"),
      this);

  // Match return of reference to local variable (via implicit cast to ref)
  Finder->addMatcher(
      returnStmt(hasReturnValue(implicitCastExpr(hasSourceExpression(
                     declRefExpr(to(varDecl(hasAutomaticStorageDuration(),
                                            unless(hasStaticStorageDuration()))
                                        .bind("ref_var")))))))
          .bind("return_ref"),
      this);

  // Match throw of address of local variable
  Finder->addMatcher(
      cxxThrowExpr(
          has(unaryOperator(hasOperatorName("&"),
                            hasUnaryOperand(declRefExpr(
                                to(varDecl(hasAutomaticStorageDuration(),
                                           unless(hasStaticStorageDuration()))
                                       .bind("throw_var")))))))
          .bind("throw_addr"),
      this);
}

void AvoidReturnLocalAddressCheck::check(
    const MatchFinder::MatchResult &Result) {

  if (const auto *ReturnAddr =
          Result.Nodes.getNodeAs<ReturnStmt>("return_addr")) {
    if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
      diag(ReturnAddr->getReturnLoc(),
           "returning address of local variable %0 with automatic storage "
           "duration")
          << Var;
      diag(Var->getLocation(), "variable declared here", DiagnosticIDs::Note);
    }
    return;
  }

  if (const auto *ReturnRef =
          Result.Nodes.getNodeAs<ReturnStmt>("return_ref")) {
    if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("ref_var")) {
      // Only flag if the function returns a reference type
      if (const auto *Func =
              Result.Context->getParents(*ReturnRef)[0].get<FunctionDecl>()) {
        if (Func && Func->getReturnType()->isReferenceType()) {
          diag(ReturnRef->getReturnLoc(),
               "returning reference to local variable %0 with automatic "
               "storage duration")
              << Var;
          diag(Var->getLocation(), "variable declared here",
               DiagnosticIDs::Note);
        }
      }
    }
    return;
  }

  if (const auto *ThrowAddr =
          Result.Nodes.getNodeAs<CXXThrowExpr>("throw_addr")) {
    if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("throw_var")) {
      diag(ThrowAddr->getThrowLoc(),
           "throwing address of local variable %0 with automatic storage "
           "duration")
          << Var;
      diag(Var->getLocation(), "variable declared here", DiagnosticIDs::Note);
    }
    return;
  }
}

} // namespace clang::tidy::automotive::cpp23

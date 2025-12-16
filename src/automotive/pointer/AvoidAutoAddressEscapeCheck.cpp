//===--- AvoidAutoAddressEscapeCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidAutoAddressEscapeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidAutoAddressEscapeCheck::registerMatchers(MatchFinder *Finder) {
  // Match return statements with address-of local variable
  Finder->addMatcher(
      returnStmt(
          hasReturnValue(ignoringParenImpCasts(
              unaryOperator(
                  hasOperatorName("&"),
                  hasUnaryOperand(declRefExpr(to(
                      varDecl(hasAutomaticStorageDuration()).bind("local")))))
                  .bind("addrof"))))
          .bind("return"),
      this);

  // Match assignment of address-of local to global/static pointer
  Finder->addMatcher(
      binaryOperator(
          isAssignmentOperator(),
          hasLHS(ignoringParenImpCasts(
              declRefExpr(to(varDecl(hasGlobalStorage()).bind("global"))))),
          hasRHS(ignoringParenImpCasts(unaryOperator(
              hasOperatorName("&"),
              hasUnaryOperand(declRefExpr(to(
                  varDecl(hasAutomaticStorageDuration()).bind("local2"))))))))
          .bind("assign"),
      this);

  // Match initialization of global/static pointer with address-of local
  // Note: This is harder to match since the local wouldn't exist at global
  // init time, but catch the pattern for static local pointers
  Finder->addMatcher(varDecl(hasGlobalStorage(), hasType(pointerType()),
                             hasInitializer(ignoringParenImpCasts(unaryOperator(
                                 hasOperatorName("&"),
                                 hasUnaryOperand(declRefExpr(to(varDecl(
                                     hasAutomaticStorageDuration()))))))))
                         .bind("staticinit"),
                     this);
}

void AvoidAutoAddressEscapeCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Case 1: Return address of local variable
  if (const auto *Return = Result.Nodes.getNodeAs<ReturnStmt>("return")) {
    if (Result.SourceManager->isInSystemHeader(Return->getBeginLoc()))
      return;

    const auto *Local = Result.Nodes.getNodeAs<VarDecl>("local");
    if (Local) {
      diag(Return->getBeginLoc(),
           "address of automatic variable %0 returned; object will not exist "
           "after function returns")
          << Local;
      diag(Local->getLocation(), "variable %0 declared here",
           DiagnosticIDs::Note)
          << Local;
    }
    return;
  }

  // Case 2: Assignment of local address to global pointer
  if (const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assign")) {
    if (Result.SourceManager->isInSystemHeader(Assign->getOperatorLoc()))
      return;

    const auto *Global = Result.Nodes.getNodeAs<VarDecl>("global");
    const auto *Local = Result.Nodes.getNodeAs<VarDecl>("local2");
    if (Global && Local) {
      diag(Assign->getOperatorLoc(),
           "address of automatic variable %0 assigned to object %1 with "
           "greater lifetime")
          << Local << Global;
      diag(Local->getLocation(), "automatic variable %0 declared here",
           DiagnosticIDs::Note)
          << Local;
    }
    return;
  }

  // Case 3: Static/global initialized with address of local
  if (const auto *StaticInit = Result.Nodes.getNodeAs<VarDecl>("staticinit")) {
    if (Result.SourceManager->isInSystemHeader(StaticInit->getLocation()))
      return;

    diag(StaticInit->getLocation(),
         "static or global pointer %0 initialized with address of automatic "
         "variable")
        << StaticInit;
  }
}

} // namespace clang::tidy::automotive

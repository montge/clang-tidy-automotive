//===--- FunctionDeclarationMismatchCheck.cpp - clang-tidy ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "FunctionDeclarationMismatchCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void FunctionDeclarationMismatchCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(isDefinition()).bind("funcDecl"), this);
}

void FunctionDeclarationMismatchCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl");

  if (!FuncDecl || !FuncDecl->getPreviousDecl())
    return;

  // Skip C++ special member functions (constructors, destructors, operators)
  // which don't have simple identifier names
  if (!FuncDecl->getDeclName().isIdentifier())
    return;

  const auto *PrevDecl = FuncDecl->getPreviousDecl();

  if (FuncDecl->getType().getCanonicalType().getUnqualifiedType() !=
      PrevDecl->getType().getCanonicalType().getUnqualifiedType()) {
    diag(FuncDecl->getLocation(),
         "Inconsistent function declaration types for '%0'")
        << FuncDecl->getName();
  }

  // Compare parameter types and names
  if (FuncDecl->param_size() == PrevDecl->param_size()) {
    for (unsigned i = 0; i < FuncDecl->param_size(); ++i) {
      const ParmVarDecl *ParamDecl = FuncDecl->getParamDecl(i);
      const ParmVarDecl *PrevParamDecl = PrevDecl->getParamDecl(i);

      // Compare parameter names
      if (ParamDecl->getName() != PrevParamDecl->getName()) {
        diag(ParamDecl->getLocation(),
             "parameter name '%0' mismatch with '%1' in function '%2'")
            << ParamDecl->getName() << PrevParamDecl->getName()
            << FuncDecl->getName();
        diag(PrevParamDecl->getLocation(), "position of '%0'",
             DiagnosticIDs::Note)
            << PrevParamDecl->getName();
      }

      // Compare parameter type qualifiers
      if (ParamDecl->getType().getCanonicalType() !=
          PrevParamDecl->getType().getCanonicalType()) {
        diag(FuncDecl->getLocation(),
             "Mismatched type qualifiers between declarations of function '%0'")
            << FuncDecl->getName();
      }
    }
  }
}

} // namespace clang::tidy::automotive

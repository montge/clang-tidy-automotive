//===--- DefaultArgumentOrderCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DefaultArgumentOrderCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void DefaultArgumentOrderCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match function declarations (not definitions) that have parameters
  Finder->addMatcher(
      functionDecl(unless(isDefinition()), unless(isImplicit())).bind("func"),
      this);
}

// Helper function to check if two default argument expressions are equivalent
static bool areDefaultArgsEquivalent(const Expr *Arg1, const Expr *Arg2,
                                     ASTContext *Context) {
  if (!Arg1 || !Arg2)
    return false;

  // Strip implicit casts and parentheses for comparison
  Arg1 = Arg1->IgnoreParenImpCasts();
  Arg2 = Arg2->IgnoreParenImpCasts();

  // Use the Stmt profile for structural comparison
  llvm::FoldingSetNodeID ID1, ID2;
  Arg1->Profile(ID1, *Context, true);
  Arg2->Profile(ID2, *Context, true);

  return ID1 == ID2;
}

void DefaultArgumentOrderCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!FuncDecl)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(FuncDecl->getLocation()))
    return;

  // Get the previous declaration if it exists
  const FunctionDecl *PrevDecl = FuncDecl->getPreviousDecl();
  if (!PrevDecl)
    return;

  // Check if this is a C++ function (skip C functions in extern "C" blocks)
  if (!isa<CXXMethodDecl>(FuncDecl) && FuncDecl->isExternC())
    return;

  // Compare default arguments between this declaration and previous ones
  unsigned NumParams = FuncDecl->getNumParams();
  unsigned PrevNumParams = PrevDecl->getNumParams();

  // Should have the same number of parameters (guaranteed by overload
  // resolution)
  if (NumParams != PrevNumParams)
    return;

  // Check each parameter for default argument redefinition
  for (unsigned i = 0; i < NumParams; ++i) {
    const ParmVarDecl *Param = FuncDecl->getParamDecl(i);
    const ParmVarDecl *PrevParam = PrevDecl->getParamDecl(i);

    // Check if both have default arguments
    if (Param->hasDefaultArg() && PrevParam->hasDefaultArg()) {
      const Expr *DefaultArg = Param->getDefaultArg();
      const Expr *PrevDefaultArg = PrevParam->getDefaultArg();

      // Skip if either is an uninstantiated default argument
      if (!DefaultArg || !PrevDefaultArg)
        continue;

      // Check if the default arguments are different
      if (!areDefaultArgsEquivalent(DefaultArg, PrevDefaultArg,
                                    Result.Context)) {
        diag(Param->getDefaultArg()->getBeginLoc(),
             "default argument for parameter '%0' of function '%1' differs "
             "from previous declaration")
            << Param->getNameAsString() << FuncDecl->getQualifiedNameAsString();

        diag(PrevParam->getDefaultArg()->getBeginLoc(),
             "previous default argument specified here", DiagnosticIDs::Note);
      }
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

//===--- UninitializedAutoVarCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UninitializedAutoVarCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void UninitializedAutoVarCheck::registerMatchers(MatchFinder *Finder) {
  // Match local variables without initialization
  Finder->addMatcher(varDecl(hasLocalStorage(), unless(hasInitializer(expr())),
                             unless(parmVarDecl()), unless(isImplicit()))
                         .bind("var"),
                     this);
}

void UninitializedAutoVarCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!Var)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Var->getLocation()))
    return;

  // Skip extern declarations (they don't have definitions)
  if (Var->hasExternalStorage())
    return;

  // Skip static local variables (they are zero-initialized by default)
  if (Var->isStaticLocal())
    return;

  // Get the type
  QualType Type = Var->getType();

  // Skip array types (they may be initialized element-by-element)
  if (Type->isArrayType())
    return;

  // Skip types with non-trivial default constructors (they are initialized)
  if (Type->isRecordType()) {
    const CXXRecordDecl *RD = Type->getAsCXXRecordDecl();
    if (RD && RD->hasDefinition()) {
      // If the class has a default constructor (explicit or implicit),
      // the variable is implicitly initialized
      if (RD->hasDefaultConstructor())
        return;
    }
  }

  // Skip reference types (must be initialized - compiler enforces)
  if (Type->isReferenceType())
    return;

  diag(Var->getLocation(),
       "local variable '%0' is not initialized at declaration; "
       "automatic storage duration variables should be initialized")
      << Var->getName();
}

} // namespace clang::tidy::automotive

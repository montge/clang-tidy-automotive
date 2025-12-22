//===--- UseUnnamedNamespaceCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseUnnamedNamespaceCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void UseUnnamedNamespaceCheck::registerMatchers(MatchFinder *Finder) {
  // Match static functions at file scope (not class members)
  Finder->addMatcher(functionDecl(isStaticStorageClass(),
                                  unless(isExpansionInSystemHeader()),
                                  unless(cxxMethodDecl()))
                         .bind("static_func"),
                     this);

  // Match static variables at file scope
  Finder->addMatcher(varDecl(isStaticStorageClass(),
                             unless(isExpansionInSystemHeader()),
                             hasGlobalStorage(), unless(isStaticLocal()))
                         .bind("static_var"),
                     this);
}

void UseUnnamedNamespaceCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("static_func")) {
    // Skip if already in an unnamed namespace
    if (Func->isInAnonymousNamespace())
      return;

    // Skip if it's a definition inside a class (static member function)
    if (isa<CXXMethodDecl>(Func))
      return;

    diag(Func->getLocation(),
         "use an unnamed namespace instead of 'static' for internal linkage")
        << Func->getName();
    return;
  }

  if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("static_var")) {
    // Skip if already in an unnamed namespace
    if (Var->isInAnonymousNamespace())
      return;

    // Skip static class members
    if (Var->isStaticDataMember())
      return;

    // Skip local static variables (they serve a different purpose)
    if (Var->isStaticLocal())
      return;

    diag(Var->getLocation(),
         "use an unnamed namespace instead of 'static' for internal linkage")
        << Var->getName();
  }
}

} // namespace clang::tidy::automotive::cpp23

//===--- AvoidStaticInitAllocationCheck.cpp - clang-tidy ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidStaticInitAllocationCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidStaticInitAllocationCheck::registerMatchers(MatchFinder *Finder) {
  // Match new expressions
  Finder->addMatcher(cxxNewExpr().bind("new_expr"), this);

  // Match array new expressions
  Finder->addMatcher(cxxNewExpr(isArray()).bind("array_new"), this);

  // Match calls to operator new
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("operator new"))))
          .bind("operator_new"),
      this);

  // Match calls to malloc, calloc, realloc
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("malloc", "calloc", "realloc"))))
          .bind("c_alloc"),
      this);
}

bool AvoidStaticInitAllocationCheck::isInStaticInitializer(
    const Expr *E, ASTContext *Context) const {

  // Traverse up the AST to find if we're in a static variable initializer
  DynTypedNodeList Parents = Context->getParents(*E);

  while (!Parents.empty()) {
    const DynTypedNode &Parent = Parents[0];

    // Check if we're in a variable declaration
    if (const auto *VD = Parent.get<VarDecl>()) {
      // Check if it's a global or static variable
      if (VD->hasGlobalStorage() || VD->isStaticLocal()) {
        return true;
      }
      // If it's a local non-static variable, we're not in static init
      return false;
    }

    // Check if we're in a field declaration
    // Note: FieldDecl is never static; static data members are VarDecl
    if (Parent.get<FieldDecl>()) {
      // Non-static field, not in static init context
      return false;
    }

    // Continue traversing up
    Parents = Context->getParents(Parent);
  }

  return false;
}

void AvoidStaticInitAllocationCheck::check(
    const MatchFinder::MatchResult &Result) {

  const Expr *AllocExpr = nullptr;
  StringRef AllocType;

  if (const auto *NewExpr = Result.Nodes.getNodeAs<CXXNewExpr>("new_expr")) {
    AllocExpr = NewExpr;
    AllocType = "new";
  } else if (const auto *ArrayNew =
                 Result.Nodes.getNodeAs<CXXNewExpr>("array_new")) {
    AllocExpr = ArrayNew;
    AllocType = "new[]";
  } else if (const auto *OpNew =
                 Result.Nodes.getNodeAs<CallExpr>("operator_new")) {
    AllocExpr = OpNew;
    AllocType = "operator new";
  } else if (const auto *CAlloc =
                 Result.Nodes.getNodeAs<CallExpr>("c_alloc")) {
    AllocExpr = CAlloc;
    AllocType = "malloc/calloc/realloc";
  }

  if (!AllocExpr)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(AllocExpr->getBeginLoc()))
    return;

  // Check if this allocation is in a static initializer
  if (isInStaticInitializer(AllocExpr, Result.Context)) {
    diag(AllocExpr->getBeginLoc(),
         "dynamic memory allocation (%0) shall not occur during static "
         "initialization (MISRA C++:2023 Rule 10.1)")
        << AllocType;
  }
}

} // namespace clang::tidy::automotive::cpp23

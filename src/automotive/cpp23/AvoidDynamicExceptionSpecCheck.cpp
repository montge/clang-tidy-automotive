//===--- AvoidDynamicExceptionSpecCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidDynamicExceptionSpecCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidDynamicExceptionSpecCheck::registerMatchers(MatchFinder *Finder) {
  // Match function declarations with exception specifications
  Finder->addMatcher(functionDecl().bind("func"), this);
}

void AvoidDynamicExceptionSpecCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!Func)
    return;

  // Get the exception specification type
  const auto *FPT = Func->getType()->getAs<FunctionProtoType>();
  if (!FPT)
    return;

  ExceptionSpecificationType EST = FPT->getExceptionSpecType();

  // Check for dynamic exception specifications
  // EST_Dynamic: throw(type-list)
  // EST_DynamicNone: throw() - this is actually OK (equivalent to noexcept)
  // EST_MSAny: throw(...) - Microsoft extension
  if (EST == EST_Dynamic || EST == EST_MSAny) {
    diag(Func->getLocation(),
         "dynamic exception specification is deprecated; use 'noexcept' or "
         "'noexcept(false)' instead")
        << Func->getName();
  }
}

} // namespace clang::tidy::automotive::cpp23

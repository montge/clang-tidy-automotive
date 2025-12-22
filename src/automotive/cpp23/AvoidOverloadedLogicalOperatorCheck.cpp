//===--- AvoidOverloadedLogicalOperatorCheck.cpp - clang-tidy -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidOverloadedLogicalOperatorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidOverloadedLogicalOperatorCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match operator&& and operator|| declarations (member or non-member)
  Finder->addMatcher(cxxMethodDecl(anyOf(hasOverloadedOperatorName("&&"),
                                         hasOverloadedOperatorName("||")))
                         .bind("method"),
                     this);

  Finder->addMatcher(functionDecl(anyOf(hasOverloadedOperatorName("&&"),
                                        hasOverloadedOperatorName("||")),
                                  unless(cxxMethodDecl()))
                         .bind("function"),
                     this);
}

void AvoidOverloadedLogicalOperatorCheck::check(
    const MatchFinder::MatchResult &Result) {

  if (const auto *Method = Result.Nodes.getNodeAs<CXXMethodDecl>("method")) {
    StringRef OpName = Method->getOverloadedOperator() == OO_AmpAmp
                           ? "operator&&"
                           : "operator||";
    diag(Method->getLocation(),
         "overloading %0 disables short-circuit evaluation; "
         "consider alternative design")
        << OpName;
  }

  if (const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
    StringRef OpName = Func->getOverloadedOperator() == OO_AmpAmp
                           ? "operator&&"
                           : "operator||";
    diag(Func->getLocation(),
         "overloading %0 disables short-circuit evaluation; "
         "consider alternative design")
        << OpName;
  }
}

} // namespace clang::tidy::automotive::cpp23

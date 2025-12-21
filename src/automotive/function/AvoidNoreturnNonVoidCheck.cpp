//===--- AvoidNoreturnNonVoidCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidNoreturnNonVoidCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidNoreturnNonVoidCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(functionDecl().bind("func"), this);
}

void AvoidNoreturnNonVoidCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!FD)
    return;

  if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
    return;

  // Check if function has noreturn attribute
  if (!FD->isNoReturn())
    return;

  // Check if return type is void
  QualType RetType = FD->getReturnType();
  if (RetType->isVoidType())
    return;

  diag(FD->getLocation(), "_Noreturn function shall have void return type");
}

} // namespace clang::tidy::automotive

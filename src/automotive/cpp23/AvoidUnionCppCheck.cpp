//===--- AvoidUnionCppCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnionCppCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidUnionCppCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(recordDecl(isUnion()).bind("union"), this);
}

void AvoidUnionCppCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Union = Result.Nodes.getNodeAs<RecordDecl>("union");
  if (!Union)
    return;

  if (Result.SourceManager->isInSystemHeader(Union->getLocation()))
    return;

  diag(Union->getLocation(), "union shall not be used");
}

} // namespace clang::tidy::automotive

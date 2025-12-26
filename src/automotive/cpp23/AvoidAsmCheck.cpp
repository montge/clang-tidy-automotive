//===--- AvoidAsmCheck.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidAsmCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidAsmCheck::registerMatchers(MatchFinder *Finder) {
  // Match all asm statements (includes GCCAsmStmt and MSAsmStmt)
  Finder->addMatcher(asmStmt().bind("asm"), this);
}

void AvoidAsmCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *AS = Result.Nodes.getNodeAs<AsmStmt>("asm");
  if (!AS)
    return;

  SourceLocation Loc = AS->getAsmLoc();
  if (!Loc.isValid())
    return;

  if (Result.SourceManager->isInSystemHeader(Loc))
    return;

  diag(Loc, "the asm declaration shall not be used");
}

} // namespace clang::tidy::automotive::cpp23

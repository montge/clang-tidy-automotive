//===--- ImplicitIntCheck.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ImplicitIntCheck.h"
#include "../utils/TokenRange.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/AttrKinds.h"
#include "clang/Basic/CharInfo.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Lex/Lexer.h"
#include "llvm/ADT/STLExtras.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ImplicitIntCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl().bind("var"), this);
  IdentTable = std::make_unique<IdentifierTable>(getLangOpts());
}

void ImplicitIntCheck::check(const MatchFinder::MatchResult &Result) {
  auto *MatchedVar = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!MatchedVar)
    return;

  SourceLocation StartLoc = MatchedVar->getBeginLoc();
  SourceLocation EndLoc = MatchedVar->getEndLoc();

  checkImplicitInt(StartLoc, EndLoc, *Result.SourceManager, *Result.Context);
}

void ImplicitIntCheck::checkImplicitInt(SourceLocation StartLoc,
                                        SourceLocation EndLoc,
                                        const SourceManager &SM,
                                        const ASTContext &Context) {
  // Suppress unused parameter warnings for incomplete implementation.
  (void)StartLoc;
  (void)EndLoc;
  (void)SM;
  (void)Context;
  // TODO: Implement implicit int detection
}

} // namespace clang::tidy::automotive

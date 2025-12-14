//===--- AvoidLanguageExtensionCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidLanguageExtensionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidLanguageExtensionCheck::registerMatchers(MatchFinder *Finder) {
  // Match GNU statement expressions
  Finder->addMatcher(stmtExpr().bind("stmtexpr"), this);

  // Match declarations with aligned attribute
  Finder->addMatcher(decl(hasAttr(attr::Kind::Aligned)).bind("alignedattr"),
                     this);

  // Match declarations with packed attribute
  Finder->addMatcher(decl(hasAttr(attr::Kind::Packed)).bind("packedattr"),
                     this);

  // Match __builtin calls
  Finder->addMatcher(callExpr(callee(functionDecl(matchesName("^__builtin_"))))
                         .bind("builtin"),
                     this);
}

void AvoidLanguageExtensionCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Check statement expressions
  if (const auto *SE = Result.Nodes.getNodeAs<StmtExpr>("stmtexpr")) {
    if (Result.SourceManager->isInSystemHeader(SE->getBeginLoc()))
      return;

    diag(SE->getBeginLoc(),
         "statement expression is a GCC/Clang extension; avoid for "
         "portability");
    return;
  }

  // Check aligned attribute
  if (const auto *D = Result.Nodes.getNodeAs<Decl>("alignedattr")) {
    if (Result.SourceManager->isInSystemHeader(D->getLocation()))
      return;

    diag(D->getLocation(),
         "__attribute__((aligned)) is a compiler extension; use _Alignas in "
         "C11 or alignas in C++11 for portability");
    return;
  }

  // Check packed attribute
  if (const auto *D = Result.Nodes.getNodeAs<Decl>("packedattr")) {
    if (Result.SourceManager->isInSystemHeader(D->getLocation()))
      return;

    diag(D->getLocation(),
         "__attribute__((packed)) is a compiler extension and not portable");
    return;
  }

  // Check __builtin_ calls
  if (const auto *CE = Result.Nodes.getNodeAs<CallExpr>("builtin")) {
    if (Result.SourceManager->isInSystemHeader(CE->getBeginLoc()))
      return;

    diag(CE->getBeginLoc(),
         "__builtin_* functions are compiler extensions and not portable");
  }
}

} // namespace clang::tidy::automotive

//===--- AvoidZeroAlignmentCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidZeroAlignmentCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidZeroAlignmentCheck::registerMatchers(MatchFinder *Finder) {
  // Match declarations that have alignment attributes
  Finder->addMatcher(varDecl(hasAttr(attr::Aligned)).bind("var"), this);
  Finder->addMatcher(fieldDecl(hasAttr(attr::Aligned)).bind("field"), this);
  Finder->addMatcher(typedefDecl(hasAttr(attr::Aligned)).bind("typedef"), this);
  Finder->addMatcher(recordDecl(hasAttr(attr::Aligned)).bind("record"), this);
}

void AvoidZeroAlignmentCheck::check(const MatchFinder::MatchResult &Result) {
  const Decl *D = nullptr;
  SourceLocation Loc;

  if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
    D = Var;
    Loc = Var->getLocation();
  } else if (const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("field")) {
    D = Field;
    Loc = Field->getLocation();
  } else if (const auto *Typedef =
                 Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
    D = Typedef;
    Loc = Typedef->getLocation();
  } else if (const auto *Record =
                 Result.Nodes.getNodeAs<RecordDecl>("record")) {
    D = Record;
    Loc = Record->getLocation();
  }

  if (!D)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Loc))
    return;

  // Check all AlignedAttr on this declaration
  for (const auto *Attr : D->specific_attrs<AlignedAttr>()) {
    // Check if the alignment value is zero
    if (!Attr->isAlignmentExpr())
      continue;

    const Expr *AlignExpr = Attr->getAlignmentExpr();
    if (!AlignExpr)
      continue;

    Expr::EvalResult EvalResult;
    if (AlignExpr->EvaluateAsInt(EvalResult, *Result.Context)) {
      llvm::APSInt AlignValue = EvalResult.Val.getInt();
      if (AlignValue == 0) {
        diag(Attr->getLocation(),
             "alignment specifier with value zero results in undefined "
             "behavior");
      }
    }
  }
}

} // namespace clang::tidy::automotive

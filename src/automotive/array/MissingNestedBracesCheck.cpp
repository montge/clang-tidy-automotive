//===--- MissingNestedBracesCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MissingNestedBracesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void MissingNestedBracesCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with initializer lists
  Finder->addMatcher(
      varDecl(hasInitializer(initListExpr().bind("init"))).bind("var"), this);
}

void MissingNestedBracesCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *Init = Result.Nodes.getNodeAs<InitListExpr>("init");
  if (!VD || !Init)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  // Get the syntactic form (what user wrote) and semantic form (expanded)
  const InitListExpr *SyntacticForm =
      Init->isSyntacticForm() ? Init : Init->getSyntacticForm();
  const InitListExpr *SemanticForm =
      Init->isSemanticForm() ? Init : Init->getSemanticForm();

  if (!SyntacticForm)
    SyntacticForm = Init;
  if (!SemanticForm)
    SemanticForm = Init;

  QualType VarType = VD->getType().getCanonicalType();

  // Only check struct/union types (not simple arrays)
  const auto *RT = VarType->getAs<RecordType>();
  if (!RT)
    return;

  const RecordDecl *RD = RT->getDecl();
  if (!RD->isCompleteDefinition())
    return;

  // Check each field - if a field is a nested aggregate, verify it has braces
  unsigned SyntacticIdx = 0;
  for (const auto *Field : RD->fields()) {
    QualType FieldType = Field->getType().getCanonicalType();

    // Check if field is a nested aggregate (struct/array)
    bool IsNestedAggregate =
        FieldType->isRecordType() || FieldType->isArrayType();

    if (!IsNestedAggregate)
      continue;

    // Find the corresponding syntactic initializer
    if (SyntacticIdx >= SyntacticForm->getNumInits())
      break;

    const Expr *SyntacticInit = SyntacticForm->getInit(SyntacticIdx);

    // If the syntactic form doesn't have an InitListExpr for this nested
    // aggregate, the braces are missing
    if (!isa<InitListExpr>(SyntacticInit->IgnoreParenImpCasts())) {
      diag(SyntacticInit->getBeginLoc(),
           "nested aggregate initializer should be enclosed in braces")
          << FixItHint::CreateInsertion(SyntacticInit->getBeginLoc(), "{")
          << FixItHint::CreateInsertion(
                 SyntacticInit->getEndLoc().getLocWithOffset(1), "}");
      return; // Report only the first violation per initializer
    }

    ++SyntacticIdx;
  }
}

} // namespace clang::tidy::automotive

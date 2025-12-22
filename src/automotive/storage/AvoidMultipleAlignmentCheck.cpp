//===--- AvoidMultipleAlignmentCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidMultipleAlignmentCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidMultipleAlignmentCheck::registerMatchers(MatchFinder *Finder) {
  // Match declarations that have alignment attributes
  Finder->addMatcher(varDecl(hasAttr(attr::Aligned)).bind("var"), this);
  Finder->addMatcher(fieldDecl(hasAttr(attr::Aligned)).bind("field"), this);
  Finder->addMatcher(recordDecl(hasAttr(attr::Aligned)).bind("record"), this);
}

void AvoidMultipleAlignmentCheck::check(
    const MatchFinder::MatchResult &Result) {
  const Decl *D = nullptr;
  SourceLocation Loc;

  if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
    D = Var;
    Loc = Var->getLocation();
  } else if (const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("field")) {
    D = Field;
    Loc = Field->getLocation();
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

  // Count alignment attributes
  unsigned AlignAttrCount = 0;
  SourceLocation SecondAlignLoc;

  for (const auto *Attr : D->specific_attrs<AlignedAttr>()) {
    AlignAttrCount++;
    if (AlignAttrCount == 2) {
      SecondAlignLoc = Attr->getLocation();
    }
  }

  if (AlignAttrCount > 1) {
    diag(SecondAlignLoc, "multiple alignment specifiers on single declaration; "
                         "use only one alignment specifier per declaration");
    diag(Loc, "declaration with multiple alignment specifiers",
         DiagnosticIDs::Note);
  }
}

} // namespace clang::tidy::automotive

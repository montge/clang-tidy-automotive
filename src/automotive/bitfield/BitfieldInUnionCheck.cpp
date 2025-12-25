//===--- BitfieldInUnionCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "BitfieldInUnionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void BitfieldInUnionCheck::registerMatchers(MatchFinder *Finder) {
  // MISRA C:2025 Rule 6.3: A bit-field shall not be declared as a member of a
  // union
  Finder->addMatcher(
      fieldDecl(allOf(isBitField(), hasParent(recordDecl(isUnion()))))
          .bind("bitfield"),
      this);
}

void BitfieldInUnionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedField = Result.Nodes.getNodeAs<FieldDecl>("bitfield");

  if (!MatchedField) {
    return;
  }

  diag(MatchedField->getLocation(),
       "bit-field shall not be declared as a member of a union");
}

} // namespace clang::tidy::automotive

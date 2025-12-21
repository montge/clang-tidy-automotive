//===--- WrongBitfieldTypeCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "WrongBitfieldTypeCheck.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void WrongBitfieldTypeCheck::registerMatchers(MatchFinder *Finder) {
  // MISRA Rule 6.1: Bit-fields shall only be declared with an appropriate type
  // Allowed types: signed int, unsigned int, _Bool
  // Note: "signed int" and "int" have the same canonical type which prints as "int"
  Finder->addMatcher(
      fieldDecl(allOf(isBitField(),
                      unless(hasType(qualType(hasCanonicalType(anyOf(
                          asString("int"), asString("unsigned int")))))),
                      unless(hasType(booleanType()))))
          .bind("bitField"),
      this);
}

void WrongBitfieldTypeCheck::check(const MatchFinder::MatchResult &Result) {

  const auto *MatchedBitField = Result.Nodes.getNodeAs<FieldDecl>("bitField");

  if (!MatchedBitField) {
    return;
  }

  diag(MatchedBitField->getLocation(), "wrong type in bitfield");
}

} // namespace clang::tidy::automotive

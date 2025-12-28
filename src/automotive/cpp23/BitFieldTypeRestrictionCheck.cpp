//===--- BitFieldTypeRestrictionCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "BitFieldTypeRestrictionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void BitFieldTypeRestrictionCheck::registerMatchers(MatchFinder *Finder) {
  // Match field declarations that are bit-fields
  Finder->addMatcher(fieldDecl(isBitField()).bind("bitfield"), this);
}

bool BitFieldTypeRestrictionCheck::isApprovedBitFieldType(QualType Type) const {
  // Remove any qualifiers (const, volatile)
  Type = Type.getUnqualifiedType();

  // Allow bool
  if (Type->isBooleanType())
    return true;

  // Allow unsigned integer types
  if (Type->isUnsignedIntegerType())
    return true;

  // Reject everything else (signed integers, floating point, etc.)
  return false;
}

void BitFieldTypeRestrictionCheck::check(
    const MatchFinder::MatchResult &Result) {

  const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("bitfield");
  if (!Field)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Field->getLocation()))
    return;

  QualType FieldType = Field->getType();

  if (!isApprovedBitFieldType(FieldType)) {
    diag(Field->getLocation(),
         "bit-field type shall be bool or unsigned integer type; found %0 "
         "(MISRA C++:2023 Rule 12.2)")
        << FieldType;
  }
}

} // namespace clang::tidy::automotive::cpp23

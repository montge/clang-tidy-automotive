//===--- UniqueEnumValuesCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UniqueEnumValuesCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void UniqueEnumValuesCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(enumDecl().bind("enum"), this);
}

void UniqueEnumValuesCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedEnum = Result.Nodes.getNodeAs<EnumDecl>("enum");
  if (!MatchedEnum)
    return;

  llvm::DenseMap<int64_t, const EnumConstantDecl *> UsedValues;
  int64_t LastValue = -1;

  for (const auto *EnumConst : MatchedEnum->enumerators()) {
    int64_t CurrentValue = EnumConst->getInitExpr()
                               ? EnumConst->getInitVal().getSExtValue()
                               : LastValue + 1;

    auto It = UsedValues.find(CurrentValue);
    if (It != UsedValues.end()) {
      const EnumConstantDecl *PreviousEnum = It->second;
      diag(EnumConst->getLocation(), "duplicate enum value '%0' from '%1'")
          << CurrentValue << PreviousEnum->getName();
      diag(PreviousEnum->getLocation(), "declaration of '%0'",
           DiagnosticIDs::Note)
          << PreviousEnum->getName();
    } else {
      UsedValues[CurrentValue] = EnumConst;
    }

    LastValue = CurrentValue;
  }
}

} // namespace clang::tidy::automotive

//===--- ExplicitEnumTypeCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExplicitEnumTypeCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ExplicitEnumTypeCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match enum declarations that do not have a fixed underlying type
  Finder->addMatcher(enumDecl(unless(isExpansionInSystemHeader())).bind("enum"),
                     this);
}

void ExplicitEnumTypeCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Enum = Result.Nodes.getNodeAs<EnumDecl>("enum");
  if (!Enum)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Enum->getLocation()))
    return;

  // Check if the enum has a fixed underlying type
  // In C++11 and later, enums can have explicit underlying types
  // For enum class, it defaults to int if not specified, but we still
  // want to require explicit specification per MISRA C++:2023
  if (!Enum->isFixed()) {
    // Old-style enum without explicit underlying type
    diag(Enum->getLocation(),
         "enumeration underlying type shall be explicitly specified");
  } else if (Enum->isScoped()) {
    // For scoped enums (enum class), check if the type was explicitly written
    // If there's no integer type source info, it means the type was defaulted
    if (!Enum->getIntegerTypeSourceInfo()) {
      diag(Enum->getLocation(),
           "enumeration underlying type shall be explicitly specified");
    }
  }
}

} // namespace clang::tidy::automotive

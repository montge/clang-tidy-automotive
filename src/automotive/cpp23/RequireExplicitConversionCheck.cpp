//===--- RequireExplicitConversionCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RequireExplicitConversionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void RequireExplicitConversionCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus11)
    return;

  // Match conversion operators that are not explicit
  Finder->addMatcher(cxxConversionDecl(unless(isExplicit())).bind("conversion"),
                     this);
}

void RequireExplicitConversionCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Conversion =
      Result.Nodes.getNodeAs<CXXConversionDecl>("conversion");
  if (!Conversion)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Conversion->getLocation()))
    return;

  // Skip implicit/compiler-generated conversions
  if (Conversion->isImplicit())
    return;

  // Skip deleted conversions
  if (Conversion->isDeleted())
    return;

  // Get the conversion target type for the message
  QualType ConvType = Conversion->getConversionType();

  diag(Conversion->getLocation(),
       "conversion function to %0 shall be marked explicit to prevent "
       "implicit conversions")
      << ConvType
      << FixItHint::CreateInsertion(Conversion->getLocation(), "explicit ");
}

} // namespace clang::tidy::automotive::cpp23

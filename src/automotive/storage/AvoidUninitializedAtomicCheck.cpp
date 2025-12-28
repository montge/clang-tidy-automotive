//===--- AvoidUninitializedAtomicCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUninitializedAtomicCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidUninitializedAtomicCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with atomic type that lack an initializer
  // We match atomic types by checking for _Atomic qualifier or atomic_* types
  Finder->addMatcher(
      varDecl(unless(hasInitializer(anything()))).bind("atomic_var"), this);
}

void AvoidUninitializedAtomicCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("atomic_var");
  if (!VD)
    return;

  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  // Skip extern declarations (they're initialized elsewhere)
  if (VD->hasExternalStorage())
    return;

  // Skip parameter declarations
  if (isa<ParmVarDecl>(VD))
    return;

  // Check if type is atomic (has _Atomic qualifier or is an atomic_* type)
  QualType Type = VD->getType().getCanonicalType();
  if (!Type->isAtomicType())
    return;

  diag(VD->getLocation(),
       "atomic object '%0' shall be initialized before being accessed")
      << VD->getName();
}

} // namespace clang::tidy::automotive

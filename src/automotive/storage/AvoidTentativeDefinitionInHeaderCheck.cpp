//===--- AvoidTentativeDefinitionInHeaderCheck.cpp - clang-tidy ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTentativeDefinitionInHeaderCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidTentativeDefinitionInHeaderCheck::registerMatchers(
    MatchFinder *Finder) {
  // Match variable declarations at file scope that are tentative definitions
  // A tentative definition is:
  // - A declaration at file scope
  // - Without extern specifier
  // - Without initializer
  // - Not a static declaration (static in header is a different issue)
  Finder->addMatcher(
      varDecl(hasGlobalStorage(),
              unless(hasInitializer(anything())), // No initializer
              unless(isStaticStorageClass()),     // Not static
              hasDeclContext(translationUnitDecl()))
          .bind("tentative"),
      this);
}

void AvoidTentativeDefinitionInHeaderCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("tentative");
  if (!VD)
    return;

  // Check if this is in a header file
  const SourceManager &SM = *Result.SourceManager;
  SourceLocation Loc = VD->getLocation();

  if (!Loc.isValid())
    return;

  // Skip system headers
  if (SM.isInSystemHeader(Loc))
    return;

  // Check if the file is a header file
  StringRef Filename = SM.getFilename(Loc);
  if (Filename.empty())
    return;

  // Common header file extensions
  if (!Filename.ends_with(".h") && !Filename.ends_with(".hpp") &&
      !Filename.ends_with(".hxx") && !Filename.ends_with(".H") &&
      !Filename.ends_with(".hh"))
    return;

  // Check if this is a true tentative definition (not a forward declaration)
  // A tentative definition has no initializer and is not extern
  if (VD->hasExternalStorage())
    return;

  diag(VD->getLocation(),
       "tentative definition of '%0' in header file; use 'extern' for "
       "declaration or provide an initializer")
      << VD->getName();
}

} // namespace clang::tidy::automotive

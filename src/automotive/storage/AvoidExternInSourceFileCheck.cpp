//===--- AvoidExternInSourceFileCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidExternInSourceFileCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/StringRef.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidExternInSourceFileCheck::registerMatchers(MatchFinder *Finder) {
  // Match extern variable declarations at file scope
  // Only match variables with explicit extern storage class
  Finder->addMatcher(
      varDecl(hasExternalFormalLinkage(), hasParent(translationUnitDecl()))
          .bind("externVar"),
      this);

  // Match extern function declarations (not definitions) at file scope
  // Exclude static functions which have internal linkage
  Finder->addMatcher(
      functionDecl(unless(isDefinition()), unless(isStaticStorageClass()),
                   hasParent(translationUnitDecl()))
          .bind("externFunc"),
      this);
}

bool AvoidExternInSourceFileCheck::isSourceFile(StringRef FileName) const {
  return FileName.ends_with(".c") || FileName.ends_with(".cpp") ||
         FileName.ends_with(".cc") || FileName.ends_with(".cxx");
}

void AvoidExternInSourceFileCheck::check(
    const MatchFinder::MatchResult &Result) {
  SourceLocation Loc;
  StringRef DeclKind;
  StringRef DeclName;

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("externVar")) {
    // Only match explicit extern storage class
    if (!VD->hasExternalStorage())
      return;

    Loc = VD->getLocation();
    DeclKind = "variable";
    DeclName = VD->getName();
  } else if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("externFunc")) {
    // Skip if it's a definition (function definitions are not extern decls)
    if (FD->isThisDeclarationADefinition())
      return;

    // Skip implicit declarations
    if (FD->isImplicit())
      return;

    Loc = FD->getLocation();
    DeclKind = "function";
    DeclName = FD->getName();
  } else {
    return;
  }

  if (!Loc.isValid())
    return;

  const SourceManager &SM = *Result.SourceManager;

  // Skip system headers
  if (SM.isInSystemHeader(Loc))
    return;

  // Get the file name where the declaration appears
  StringRef FileName = SM.getFilename(Loc);
  if (FileName.empty())
    return;

  // Only warn for source files (not headers)
  if (!isSourceFile(FileName))
    return;

  diag(Loc,
       "external %0 declaration '%1' should be in a header file, not a source "
       "file")
      << DeclKind << DeclName;
}

} // namespace clang::tidy::automotive

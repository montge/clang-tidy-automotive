//===--- DuplicateTagNameCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DuplicateTagNameCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/STLExtras.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
/// Returns the tag kind as a string ("struct", "union", or "enum").
StringRef getTagKindName(const TagDecl *Tag) {
  if (isa<EnumDecl>(Tag))
    return "enum";
  if (const auto *RD = dyn_cast<RecordDecl>(Tag))
    return RD->isUnion() ? "union" : "struct";
  return "tag";
}
} // namespace

void DuplicateTagNameCheck::registerMatchers(MatchFinder *Finder) {
  // Match all tag declarations (struct, union, enum) that have a name
  // Exclude implicit declarations and system headers
  Finder->addMatcher(
      tagDecl(unless(isImplicit()), unless(isExpansionInSystemHeader()))
          .bind("tag"),
      this);
}

void DuplicateTagNameCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Tag = Result.Nodes.getNodeAs<TagDecl>("tag");
  SM = Result.SourceManager;

  if (!Tag)
    return;

  // Skip anonymous tags (no name)
  if (Tag->getName().empty())
    return;

  // Skip if in system header (double check)
  if (SM->isInSystemHeader(Tag->getLocation()))
    return;

  // Get the canonical declaration to avoid counting forward declarations
  // and definitions of the same tag as duplicates
  const TagDecl *CanonicalTag = Tag->getCanonicalDecl();

  // Check if we've already seen this canonical declaration
  StringRef TagName = CanonicalTag->getName();
  auto &Decls = TagNames[TagName];

  // Only add if we haven't seen this exact canonical declaration before
  if (llvm::find(Decls, CanonicalTag) == Decls.end())
    Decls.push_back(CanonicalTag);
}

void DuplicateTagNameCheck::onEndOfTranslationUnit() {
  // Check for duplicate tag names
  for (const auto &Entry : TagNames) {
    const auto &Decls = Entry.getValue();

    // If more than one canonical declaration uses this name, it's a duplicate
    if (Decls.size() > 1) {
      // Report the first duplicate and subsequent ones
      for (size_t i = 1; i < Decls.size(); ++i) {
        const TagDecl *CurrentTag = Decls[i];
        const TagDecl *FirstTag = Decls[0];

        StringRef TagKind = getTagKindName(CurrentTag);
        StringRef FirstTagKind = getTagKindName(FirstTag);

        diag(CurrentTag->getLocation(),
             "duplicate tag name '%0'; %1 '%0' conflicts with %2 declared here")
            << Entry.getKey() << TagKind << FirstTagKind;
        diag(FirstTag->getLocation(), "previous declaration of %0 '%1' is here",
             DiagnosticIDs::Note)
            << FirstTagKind << Entry.getKey();
      }
    }
  }

  // Clear for next TU
  TagNames.clear();
}

} // namespace clang::tidy::automotive

//===--- AvoidUnrelatedPointerConversionCheck.cpp - clang-tidy -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnrelatedPointerConversionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidUnrelatedPointerConversionCheck::registerMatchers(
    MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match static_cast of pointer types
  Finder->addMatcher(
      cxxStaticCastExpr(hasDestinationType(pointerType())).bind("static-cast"),
      this);

  // Match reinterpret_cast of pointer types
  Finder->addMatcher(cxxReinterpretCastExpr(hasDestinationType(pointerType()))
                         .bind("reinterpret-cast"),
                     this);
}

/// Check if two record types are related via inheritance.
/// Returns true if either:
/// - The types are the same
/// - SourceDecl is derived from DestDecl (derived-to-base)
/// - DestDecl is derived from SourceDecl (base-to-derived, allowed with
/// static_cast)
static bool areRelatedClasses(const CXXRecordDecl *SourceDecl,
                              const CXXRecordDecl *DestDecl) {
  if (!SourceDecl || !DestDecl)
    return true; // Can't determine, don't warn

  // Same class is always related
  if (SourceDecl == DestDecl)
    return true;

  // Check if source is derived from destination (derived-to-base)
  if (SourceDecl->isDerivedFrom(DestDecl))
    return true;

  // Check if destination is derived from source (base-to-derived)
  if (DestDecl->isDerivedFrom(SourceDecl))
    return true;

  return false;
}

void AvoidUnrelatedPointerConversionCheck::check(
    const MatchFinder::MatchResult &Result) {

  const CXXNamedCastExpr *Cast = nullptr;
  SourceLocation CastLoc;

  // Check for static_cast
  if (const auto *StaticCast =
          Result.Nodes.getNodeAs<CXXStaticCastExpr>("static-cast")) {
    Cast = StaticCast;
    CastLoc = StaticCast->getOperatorLoc();
  }
  // Check for reinterpret_cast
  else if (const auto *ReinterpretCast =
               Result.Nodes.getNodeAs<CXXReinterpretCastExpr>(
                   "reinterpret-cast")) {
    Cast = ReinterpretCast;
    CastLoc = ReinterpretCast->getOperatorLoc();
  }

  if (!Cast)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(CastLoc))
    return;

  // Get the destination and source types
  QualType DestType = Cast->getTypeAsWritten();
  QualType SourceType = Cast->getSubExpr()->getType();

  // Strip pointer to get the pointee types
  const PointerType *DestPtrType = DestType->getAs<PointerType>();
  const PointerType *SourcePtrType = SourceType->getAs<PointerType>();

  if (!DestPtrType || !SourcePtrType)
    return;

  QualType DestPointee = DestPtrType->getPointeeType();
  QualType SourcePointee = SourcePtrType->getPointeeType();

  // Get the class record declarations
  const CXXRecordDecl *DestDecl = DestPointee->getAsCXXRecordDecl();
  const CXXRecordDecl *SourceDecl = SourcePointee->getAsCXXRecordDecl();

  // Only check conversions between class types
  if (!DestDecl || !SourceDecl)
    return;

  // Check if the classes are in the same inheritance hierarchy
  if (!areRelatedClasses(SourceDecl, DestDecl)) {
    diag(CastLoc,
         "pointer conversion between unrelated classes '%0' and '%1' should be "
         "avoided")
        << SourceDecl->getQualifiedNameAsString()
        << DestDecl->getQualifiedNameAsString();
  }
}

} // namespace clang::tidy::automotive::cpp23

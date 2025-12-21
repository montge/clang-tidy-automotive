//===--- AvoidAtomicVoidCheck.cpp - clang-tidy ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidAtomicVoidCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidAtomicVoidCheck::registerMatchers(MatchFinder *Finder) {
  // Match any declaration with atomic void type
  Finder->addMatcher(varDecl().bind("varDecl"), this);
  Finder->addMatcher(fieldDecl().bind("fieldDecl"), this);
  Finder->addMatcher(typedefDecl().bind("typedefDecl"), this);
}

void AvoidAtomicVoidCheck::check(const MatchFinder::MatchResult &Result) {
  SourceLocation Loc;
  QualType Type;

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;
    Loc = VD->getLocation();
    Type = VD->getType();
  } else if (const auto *FD = Result.Nodes.getNodeAs<FieldDecl>("fieldDecl")) {
    if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
      return;
    Loc = FD->getLocation();
    Type = FD->getType();
  } else if (const auto *TD =
                 Result.Nodes.getNodeAs<TypedefDecl>("typedefDecl")) {
    if (Result.SourceManager->isInSystemHeader(TD->getLocation()))
      return;
    Loc = TD->getLocation();
    Type = TD->getUnderlyingType();
  } else {
    return;
  }

  // Check if the type is _Atomic void
  if (const auto *AT = Type->getAs<AtomicType>()) {
    QualType ValueType = AT->getValueType();
    if (ValueType->isVoidType()) {
      diag(Loc, "_Atomic qualifier shall not be applied to void type");
    }
  }
}

} // namespace clang::tidy::automotive

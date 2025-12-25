//===--- AvoidVirtualBaseClassCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidVirtualBaseClassCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidVirtualBaseClassCheck::registerMatchers(MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match class definitions that have base classes
  Finder->addMatcher(
      cxxRecordDecl(isDefinition(), unless(isLambda()), hasAnyBase(anything()))
          .bind("class"),
      this);
}

void AvoidVirtualBaseClassCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ClassDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("class");

  if (!ClassDecl)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(ClassDecl->getBeginLoc()))
    return;

  // Iterate over base specifiers to find virtual bases
  for (const auto &Base : ClassDecl->bases()) {
    if (Base.isVirtual()) {
      // Get the name of the base class for better diagnostics
      QualType BaseType = Base.getType();
      const auto *BaseRecordType = BaseType->getAsCXXRecordDecl();
      std::string BaseName = BaseRecordType ? BaseRecordType->getNameAsString()
                                            : BaseType.getAsString();

      diag(Base.getBeginLoc(),
           "virtual inheritance from '%0' should be avoided unless necessary")
          << BaseName;
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

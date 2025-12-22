//===--- AvoidUninitializedArrayCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUninitializedArrayCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidUninitializedArrayCheck::registerMatchers(MatchFinder *Finder) {
  // Match array variable declarations without initializers
  Finder->addMatcher(varDecl(hasType(arrayType()),
                             unless(hasInitializer(anything())),
                             unless(isExternC()), unless(hasAttr(attr::Unused)),
                             unless(parmVarDecl()))
                         .bind("uninit_array"),
                     this);
}

void AvoidUninitializedArrayCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("uninit_array");
  if (!Var)
    return;

  // Skip extern declarations (they're just declarations, not definitions)
  if (Var->hasExternalStorage())
    return;

  // Skip static class members without definition
  if (Var->isStaticDataMember() && !Var->isThisDeclarationADefinition())
    return;

  diag(Var->getLocation(),
       "array '%0' is not explicitly initialized; consider using "
       "value-initialization or an initializer list")
      << Var->getName();
}

} // namespace clang::tidy::automotive::cpp23

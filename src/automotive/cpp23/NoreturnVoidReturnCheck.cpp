//===--- NoreturnVoidReturnCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "NoreturnVoidReturnCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void NoreturnVoidReturnCheck::registerMatchers(MatchFinder *Finder) {
  // Match functions with noreturn attribute that don't have void return type
  Finder->addMatcher(
      functionDecl(anyOf(hasAttr(attr::NoReturn), hasAttr(attr::CXX11NoReturn)),
                   unless(returns(voidType())))
          .bind("noreturn_func"),
      this);
}

void NoreturnVoidReturnCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("noreturn_func");
  if (!Func)
    return;

  // Skip implicit functions
  if (Func->isImplicit())
    return;

  // Skip functions in system headers
  if (Result.Context->getSourceManager().isInSystemHeader(Func->getLocation()))
    return;

  // Skip template instantiations, we'll check the template itself
  if (Func->isTemplateInstantiation())
    return;

  QualType ReturnType = Func->getReturnType();
  std::string TypeStr = ReturnType.getAsString();

  // For typedef types, show both the typedef name and underlying type
  if (const auto *TDT = ReturnType->getAs<TypedefType>()) {
    std::string UnderlyingStr =
        TDT->getDecl()->getUnderlyingType().getAsString();
    if (TypeStr != UnderlyingStr) {
      TypeStr += " (aka '" + UnderlyingStr + "')";
    }
  }

  diag(Func->getLocation(),
       "noreturn function %0 should have void return type, but has %1")
      << Func << TypeStr;
}

} // namespace clang::tidy::automotive

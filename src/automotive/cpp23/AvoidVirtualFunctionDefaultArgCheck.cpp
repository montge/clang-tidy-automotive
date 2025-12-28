//===--- AvoidVirtualFunctionDefaultArgCheck.cpp - clang-tidy ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidVirtualFunctionDefaultArgCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidVirtualFunctionDefaultArgCheck::registerMatchers(
    MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match virtual member functions (includes override and final)
  Finder->addMatcher(
      cxxMethodDecl(isVirtual(), unless(isImplicit())).bind("method"), this);
}

void AvoidVirtualFunctionDefaultArgCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Method = Result.Nodes.getNodeAs<CXXMethodDecl>("method");
  if (!Method)
    return;

  // Skip if method has invalid location (e.g., template instantiations)
  if (Method->getLocation().isInvalid())
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Method->getLocation()))
    return;

  // Skip template instantiations - check the template pattern instead
  if (Method->isTemplateInstantiation())
    return;

  // Check each parameter for default arguments
  for (unsigned i = 0; i < Method->getNumParams(); ++i) {
    const ParmVarDecl *Param = Method->getParamDecl(i);
    if (Param->hasDefaultArg()) {
      const Expr *DefaultArg = Param->getDefaultArg();
      // Skip if default argument has invalid location
      if (!DefaultArg || DefaultArg->getBeginLoc().isInvalid())
        continue;

      // Report the issue at the parameter location
      diag(DefaultArg->getBeginLoc(),
           "virtual function '%0' has default argument for parameter '%1'")
          << Method->getQualifiedNameAsString() << Param->getNameAsString();

      // Add a note at the function declaration
      diag(Method->getLocation(), "virtual function declared here",
           DiagnosticIDs::Note);
    }
  }
}

} // namespace clang::tidy::automotive

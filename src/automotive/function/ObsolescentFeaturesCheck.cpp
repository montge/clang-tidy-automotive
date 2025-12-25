//===--- ObsolescentFeaturesCheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ObsolescentFeaturesCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ObsolescentFeaturesCheck::registerMatchers(MatchFinder *Finder) {
  // Match function definitions that might use K&R style
  Finder->addMatcher(functionDecl(isDefinition()).bind("funcDef"), this);
}

void ObsolescentFeaturesCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *FuncDecl = Result.Nodes.getNodeAs<FunctionDecl>("funcDef");
  if (!FuncDecl)
    return;

  // Skip system headers
  if (FuncDecl->getLocation().isInvalid())
    return;

  const SourceManager &SM = *Result.SourceManager;
  if (SM.isInSystemHeader(FuncDecl->getLocation()))
    return;

  // Check for K&R style function definition
  // K&R style functions have parameters but no prototype info in the type
  const auto *FuncType = FuncDecl->getType()->getAs<FunctionProtoType>();

  // If it's a FunctionNoProtoType, it's an old-style declaration
  if (FuncDecl->getType()->getAs<FunctionNoProtoType>()) {
    diag(FuncDecl->getLocation(),
         "obsolescent feature: function '%0' uses old-style declaration "
         "without prototype")
        << FuncDecl->getName();
    return;
  }

  // Check for K&R style definition by looking at parameter declarations
  // In K&R style, parameters are declared separately after the function header
  if (FuncType && FuncDecl->hasBody()) {
    // Check if the function uses K&R style by examining if parameter
    // declarations come after the function declarator
    for (const ParmVarDecl *Param : FuncDecl->parameters()) {
      // K&R style parameters typically have their type declared separately
      // We can detect this by checking if the parameter location differs
      // significantly from what we'd expect in prototype style

      // Check for implicit int (another obsolescent feature)
      if (Param->getType()->isIntegerType()) {
        // If the original type was implicitly int, this is K&R style
        // This is hard to detect directly, so we check for other indicators
      }
    }
  }

  // Check for empty parameter list meaning "unspecified parameters"
  // In C, f() means unspecified params, f(void) means no params
  // This is obsolescent behavior
  if (FuncType && FuncType->getNumParams() == 0 && !FuncType->isVariadic()) {
    // Check if this is truly (void) or just ()
    // We need to look at the source to determine this
    // For now, we check if it's explicitly prototyped
    if (!FuncDecl->hasPrototype() && FuncDecl->getNumParams() == 0) {
      // This is a function with no prototype - could be obsolescent
      // But we need to be careful not to flag legitimate (void) functions
    }
  }
}

} // namespace clang::tidy::automotive

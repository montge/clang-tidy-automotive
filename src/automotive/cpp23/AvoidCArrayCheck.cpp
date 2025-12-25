//===--- AvoidCArrayCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCArrayCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidCArrayCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with array types
  Finder->addMatcher(
      varDecl(hasType(arrayType()), unless(isExternC())).bind("c_array"), this);

  // Match parameter declarations with array types
  Finder->addMatcher(parmVarDecl(hasType(arrayType()), unless(isExternC()))
                         .bind("c_array_param"),
                     this);
}

void AvoidCArrayCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("c_array");
  const auto *Param = Result.Nodes.getNodeAs<ParmVarDecl>("c_array_param");

  const ValueDecl *Decl = Var ? static_cast<const ValueDecl *>(Var)
                              : static_cast<const ValueDecl *>(Param);
  if (!Decl)
    return;

  // Skip declarations in system headers
  if (Result.SourceManager->isInSystemHeader(Decl->getLocation()))
    return;

  // For parameters, check if this is main's argv parameter
  if (Param) {
    if (const auto *Func = dyn_cast<FunctionDecl>(Param->getDeclContext())) {
      if (Func->isMain()) {
        // Main function can have argv parameter
        return;
      }
    }
  }

  // Determine the appropriate replacement suggestion
  StringRef Replacement = "std::array or std::vector";
  if (Param) {
    Replacement = "std::span, std::vector, or a reference to std::array";
  }

  diag(Decl->getLocation(),
       "C-style array '%0' should be replaced with %1 for better safety")
      << Decl->getName() << Replacement;
}

} // namespace clang::tidy::automotive::cpp23

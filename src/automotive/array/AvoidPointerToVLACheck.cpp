//===--- AvoidPointerToVLACheck.cpp - clang-tidy -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidPointerToVLACheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidPointerToVLACheck::registerMatchers(MatchFinder *Finder) {
  // Match pointer types that point to variably-modified array types.
  // This includes:
  // - int (*p)[n] where n is a variable
  // - Multi-dimensional cases like int (*p)[n][m]
  // - Pointers to incomplete array types with variable bounds

  auto PointerToVLA = pointerType(pointee(hasCanonicalType(
      anyOf(variableArrayType(), dependentSizedArrayType()))));

  // Match variable declarations with pointer-to-VLA type
  Finder->addMatcher(varDecl(hasType(PointerToVLA)).bind("ptrToVLA"), this);

  // Match function parameters with pointer-to-VLA type
  Finder->addMatcher(parmVarDecl(hasType(PointerToVLA)).bind("ptrToVLA"), this);
}

void AvoidPointerToVLACheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("ptrToVLA");

  if (!MatchedDecl) {
    return;
  }

  diag(MatchedDecl->getLocation(),
       "pointer to variably-modified array type shall not be used");
}

} // namespace clang::tidy::automotive

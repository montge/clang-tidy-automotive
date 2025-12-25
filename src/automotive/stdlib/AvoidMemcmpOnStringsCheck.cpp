//===--- AvoidMemcmpOnStringsCheck.cpp - clang-tidy ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidMemcmpOnStringsCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void AvoidMemcmpOnStringsCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to memcmp
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasName("memcmp")))).bind("memcmpCall"),
      this);
}

void AvoidMemcmpOnStringsCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("memcmpCall");
  if (!Call || Call->getNumArgs() < 2)
    return;

  // Skip system headers
  const SourceManager &SM = *Result.SourceManager;
  if (SM.isInSystemHeader(Call->getBeginLoc()))
    return;

  const Expr *Arg0 = Call->getArg(0)->IgnoreParenImpCasts();
  const Expr *Arg1 = Call->getArg(1)->IgnoreParenImpCasts();

  // Check if either argument is a string literal
  bool Arg0IsStringLiteral = isa<StringLiteral>(Arg0);
  bool Arg1IsStringLiteral = isa<StringLiteral>(Arg1);

  // Check if either argument is a char array (potential string)
  auto isCharArrayOrPointer = [](const Expr *E) {
    QualType T = E->getType();
    if (T->isPointerType()) {
      QualType PointeeT = T->getPointeeType();
      return PointeeT->isCharType();
    }
    if (T->isArrayType()) {
      if (const auto *AT = T->getAsArrayTypeUnsafe()) {
        return AT->getElementType()->isCharType();
      }
    }
    return false;
  };

  bool Arg0IsCharArray = isCharArrayOrPointer(Arg0);
  bool Arg1IsCharArray = isCharArrayOrPointer(Arg1);

  // If both arguments are char pointers/arrays or string literals,
  // warn about potential string comparison
  if ((Arg0IsStringLiteral || Arg0IsCharArray) &&
      (Arg1IsStringLiteral || Arg1IsCharArray)) {
    diag(Call->getBeginLoc(),
         "memcmp shall not be used to compare null-terminated strings; "
         "use strcmp or strncmp instead");
  }
}

} // namespace clang::tidy::automotive

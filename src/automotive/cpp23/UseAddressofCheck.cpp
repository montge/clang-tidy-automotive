//===--- UseAddressofCheck.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UseAddressofCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void UseAddressofCheck::registerMatchers(MatchFinder *Finder) {
  // Match unary address-of operator on class/struct types
  // We only warn for class types since those can have overloaded operator&
  Finder->addMatcher(
      unaryOperator(
          hasOperatorName("&"),
          hasUnaryOperand(expr(hasType(cxxRecordDecl())).bind("operand")))
          .bind("addrof"),
      this);
}

void UseAddressofCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *AddrOf = Result.Nodes.getNodeAs<UnaryOperator>("addrof");
  const auto *Operand = Result.Nodes.getNodeAs<Expr>("operand");

  if (!AddrOf || !Operand)
    return;

  // Skip if we're already inside a std::addressof call
  // (avoid warning about the implementation of addressof itself)
  const auto &Parents = Result.Context->getParents(*AddrOf);
  for (const auto &Parent : Parents) {
    if (const auto *Call = Parent.get<CallExpr>()) {
      if (const auto *Callee = Call->getDirectCallee()) {
        if (Callee->getQualifiedNameAsString() == "std::addressof")
          return;
      }
    }
  }

  // Get the type name for the diagnostic
  QualType Type = Operand->getType();
  std::string TypeName = Type.getAsString();

  diag(AddrOf->getOperatorLoc(),
       "use 'std::addressof' instead of unary '&' for class type '%0' to "
       "avoid issues with overloaded operator&")
      << TypeName;
}

} // namespace clang::tidy::automotive::cpp23

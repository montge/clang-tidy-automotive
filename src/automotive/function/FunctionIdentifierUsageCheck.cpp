//===--- FunctionIdentifierUsageCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "FunctionIdentifierUsageCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void FunctionIdentifierUsageCheck::registerMatchers(MatchFinder *Finder) {
  // Match DeclRefExpr that refers to a function and is implicitly converted
  // to a function pointer (FunctionToPointerDecay), but is NOT:
  // 1. The direct callee of a CallExpr (function call with ())
  // 2. Inside a UnaryOperator with opcode UO_AddrOf (explicit &)

  // Match implicit function-to-pointer decay that is not from explicit &
  Finder->addMatcher(
      implicitCastExpr(
          hasCastKind(CK_FunctionToPointerDecay),
          has(declRefExpr(to(functionDecl().bind("func"))).bind("ref")),
          // Exclude if parent is UnaryOperator with & (explicit address-of)
          unless(hasParent(unaryOperator(hasOperatorName("&"))))
      ).bind("cast"),
      this);
}

void FunctionIdentifierUsageCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Cast = Result.Nodes.getNodeAs<ImplicitCastExpr>("cast");
  const auto *Ref = Result.Nodes.getNodeAs<DeclRefExpr>("ref");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

  if (!Cast || !Ref || !Func)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Ref->getLocation()))
    return;

  // Check if this cast is the callee of a CallExpr (function call with ())
  // In that case, it's compliant
  auto Parents = Result.Context->getParents(*Cast);
  if (!Parents.empty()) {
    if (const auto *CE = Parents[0].get<CallExpr>()) {
      if (CE->getCallee() == Cast) {
        // This is a function call like func(), which is compliant
        return;
      }
    }
  }

  // Report the violation
  diag(Ref->getLocation(),
       "function identifier '%0' should only be used with & or a "
       "parenthesized parameter list")
      << Func->getName();
}

} // namespace clang::tidy::automotive

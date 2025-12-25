//===--- AutoTypeRestrictionCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AutoTypeRestrictionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AutoTypeRestrictionCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with auto type that have an initializer
  Finder->addMatcher(
      varDecl(hasType(autoType()), hasInitializer(expr().bind("init")),
              unless(isImplicit()), unless(isExpansionInSystemHeader()))
          .bind("var"),
      this);
}

bool AutoTypeRestrictionCheck::isTypeObvious(const Expr *Init) const {
  if (!Init)
    return false;

  // Remove implicit casts and parentheses to get to the actual expression
  Init = Init->IgnoreParenImpCasts();

  // 1. Explicit cast expressions - type is obvious
  if (isa<CStyleCastExpr>(Init) || isa<CXXStaticCastExpr>(Init) ||
      isa<CXXReinterpretCastExpr>(Init) || isa<CXXConstCastExpr>(Init) ||
      isa<CXXDynamicCastExpr>(Init) || isa<CXXFunctionalCastExpr>(Init)) {
    return true;
  }

  // 2. new expressions - type is explicit
  if (isa<CXXNewExpr>(Init)) {
    return true;
  }

  // 3. Lambda expressions - auto is required/appropriate
  if (isa<LambdaExpr>(Init)) {
    return true;
  }

  // 4. make_unique, make_shared, and similar factory functions
  if (const auto *Call = dyn_cast<CallExpr>(Init)) {
    if (const auto *Callee = Call->getDirectCallee()) {
      const std::string CalleeName = Callee->getNameAsString();

      // Check for make_unique, make_shared, make_optional, etc.
      if (CalleeName.find("make_unique") != std::string::npos ||
          CalleeName.find("make_shared") != std::string::npos ||
          CalleeName.find("make_optional") != std::string::npos ||
          CalleeName.find("make_pair") != std::string::npos) {
        return true;
      }
    }
  }

  // 5. Check for constructor calls (explicit type construction)
  if (const auto *Construct = dyn_cast<CXXConstructExpr>(Init)) {
    // Temporary object construction like MyClass() or MyClass(args)
    if (const auto *Temporary = dyn_cast<CXXTemporaryObjectExpr>(Construct)) {
      return true;
    }

    // Functional-style cast or construction
    if (isa<CXXFunctionalCastExpr>(Init)) {
      return true;
    }
  }

  // 6. Material temporary expressions with explicit construction
  if (const auto *Materialize = dyn_cast<MaterializeTemporaryExpr>(Init)) {
    return isTypeObvious(Materialize->getSubExpr());
  }

  // 7. Bind temporary expressions
  if (const auto *Bind = dyn_cast<CXXBindTemporaryExpr>(Init)) {
    return isTypeObvious(Bind->getSubExpr());
  }

  // 8. ExprWithCleanups
  if (const auto *Cleanup = dyn_cast<ExprWithCleanups>(Init)) {
    return isTypeObvious(Cleanup->getSubExpr());
  }

  // All other cases: literals, function calls, binary operators, etc.
  // are not considered obvious
  return false;
}

void AutoTypeRestrictionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *Init = Result.Nodes.getNodeAs<Expr>("init");

  if (!Var || !Init)
    return;

  // Skip if in system header
  if (Result.SourceManager->isInSystemHeader(Var->getLocation()))
    return;

  // Skip if in a macro (difficult to analyze context)
  if (Var->getLocation().isMacroID())
    return;

  // Special case: range-based for loops - auto is appropriate
  // Check if this is a range-for loop variable by looking at the parent context
  const auto Parents = Result.Context->getParents(*Var);
  if (!Parents.empty()) {
    if (Parents[0].get<CXXForRangeStmt>()) {
      return;
    }
  }

  // Special case: structured bindings (C++17) - auto is required
  if (isa<DecompositionDecl>(Var)) {
    return;
  }

  // Check if the type is obvious from the initializer
  if (!isTypeObvious(Init)) {
    diag(Var->getLocation(),
         "use of 'auto' type specifier where type is not obvious from "
         "initializer; consider using an explicit type or a cast to make the "
         "type clear")
        << Var->getSourceRange();
  }
}

} // namespace clang::tidy::automotive::cpp23

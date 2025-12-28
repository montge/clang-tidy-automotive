//===--- TssUsageCheck.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TssUsageCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

bool TssUsageCheck::isTssFunction(StringRef FuncName) const {
  return FuncName == "tss_create" || FuncName == "tss_delete" ||
         FuncName == "tss_get" || FuncName == "tss_set";
}

void TssUsageCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to TSS functions
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName("tss_create", "tss_delete",
                                               "tss_get", "tss_set"))))
          .bind("tssCall"),
      this);
}

void TssUsageCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("tssCall");
  if (!Call)
    return;

  // Skip system headers
  const SourceManager &SM = *Result.SourceManager;
  if (SM.isInSystemHeader(Call->getBeginLoc()))
    return;

  const FunctionDecl *FD = Call->getDirectCallee();
  if (!FD)
    return;

  StringRef FuncName = FD->getName();

  // Check tss_create - must have a valid destructor or NULL
  if (FuncName == "tss_create") {
    if (Call->getNumArgs() < 2)
      return;

    const Expr *DestructorArg = Call->getArg(1)->IgnoreParenImpCasts();

    // Check if destructor is NULL (which is allowed but discouraged)
    if (const auto *IL = dyn_cast<IntegerLiteral>(DestructorArg)) {
      if (IL->getValue() == 0) {
        diag(Call->getBeginLoc(),
             "tss_create called with NULL destructor; consider providing a "
             "cleanup function to prevent resource leaks");
      }
    } else if (const auto *GNE = dyn_cast<GNUNullExpr>(DestructorArg)) {
      diag(Call->getBeginLoc(),
           "tss_create called with NULL destructor; consider providing a "
           "cleanup function to prevent resource leaks");
    }
  }

  // Check tss_get and tss_set - warn about potential issues
  if (FuncName == "tss_get" || FuncName == "tss_set") {
    if (Call->getNumArgs() < 1)
      return;

    const Expr *KeyArg = Call->getArg(0)->IgnoreParenImpCasts();

    // Check if the key is a valid tss_t variable
    if (const auto *DRE = dyn_cast<DeclRefExpr>(KeyArg)) {
      if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
        QualType T = VD->getType();
        // Check if it's declared as tss_t
        if (const auto *TD = T->getAs<TypedefType>()) {
          if (TD->getDecl()->getName() != "tss_t") {
            diag(Call->getBeginLoc(),
                 "%0 called with argument that is not of type tss_t")
                << FuncName;
          }
        }
      }
    }

    // For tss_set, check if second argument is being properly handled
    if (FuncName == "tss_set" && Call->getNumArgs() >= 2) {
      const Expr *ValueArg = Call->getArg(1)->IgnoreParenImpCasts();

      // Warn if setting a stack variable (likely to cause issues)
      if (const auto *DRE = dyn_cast<DeclRefExpr>(ValueArg)) {
        if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
          if (!VD->hasGlobalStorage() && !VD->isStaticLocal()) {
            diag(Call->getBeginLoc(),
                 "tss_set called with pointer to local variable; this may "
                 "lead to undefined behavior when the variable goes out of "
                 "scope");
            diag(VD->getLocation(), "local variable declared here",
                 DiagnosticIDs::Note);
          }
        }
      }
    }
  }

  // For tss_delete, note that it should be called to release resources
  if (FuncName == "tss_delete") {
    // This is a note that tss_delete is being used correctly
    // Additional dataflow analysis could check if every tss_create has a
    // corresponding tss_delete, but that's beyond the scope of this simple
    // check
  }
}

} // namespace clang::tidy::automotive

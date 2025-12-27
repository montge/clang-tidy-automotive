//===--- CheckFilePointerValidityCheck.cpp - clang-tidy -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CheckFilePointerValidityCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {

// Helper to check if a call is to fopen/freopen/tmpfile
AST_MATCHER(CallExpr, isFopenLikeCall) {
  if (const auto *Callee = Node.getDirectCallee()) {
    StringRef Name = Callee->getName();
    return Name == "fopen" || Name == "freopen" || Name == "tmpfile" ||
           Name == "fdopen";
  }
  return false;
}

// Helper to check if a call is to fclose
AST_MATCHER(CallExpr, isFcloseCall) {
  if (const auto *Callee = Node.getDirectCallee()) {
    return Callee->getName() == "fclose";
  }
  return false;
}

// Helper to check if a call uses FILE* parameter
AST_MATCHER(CallExpr, usesFILEPointer) {
  if (const auto *Callee = Node.getDirectCallee()) {
    StringRef Name = Callee->getName();
    // Common FILE* operations
    return Name == "fread" || Name == "fwrite" || Name == "fgetc" ||
           Name == "fputc" || Name == "fgets" || Name == "fputs" ||
           Name == "fprintf" || Name == "fscanf" || Name == "fseek" ||
           Name == "ftell" || Name == "rewind" || Name == "fflush" ||
           Name == "feof" || Name == "ferror" || Name == "clearerr" ||
           Name == "fgetpos" || Name == "fsetpos" || Name == "ungetc";
  }
  return false;
}

} // namespace

void CheckFilePointerValidityCheck::registerMatchers(MatchFinder *Finder) {
  // Match uninitialized FILE* declarations (local variables only, not
  // parameters)
  Finder->addMatcher(
      varDecl(hasType(pointerType(pointee(
                  hasDeclaration(namedDecl(hasAnyName("FILE", "_IO_FILE")))))),
              hasLocalStorage(), unless(isStaticLocal()),
              unless(hasInitializer(anything())), unless(parmVarDecl()))
          .bind("uninit_file"),
      this);

  // Match calls to fopen-like functions (return FILE*)
  Finder->addMatcher(callExpr(isFopenLikeCall()).bind("fopen_call"), this);

  // Match calls to fclose
  Finder->addMatcher(
      callExpr(isFcloseCall(),
               hasArgument(0, ignoringParenImpCasts(declRefExpr(
                                  to(varDecl().bind("closed_var"))))))
          .bind("fclose_call"),
      this);

  // Match FILE* usage in function calls
  Finder->addMatcher(callExpr(usesFILEPointer(),
                              hasAnyArgument(ignoringParenImpCasts(declRefExpr(
                                  to(varDecl().bind("used_file_var"))))))
                         .bind("file_use"),
                     this);
}

void CheckFilePointerValidityCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Check for uninitialized FILE* declarations
  if (const auto *UninitFile = Result.Nodes.getNodeAs<VarDecl>("uninit_file")) {
    diag(UninitFile->getLocation(),
         "FILE* variable %0 declared without initialization; "
         "using an uninitialized FILE* is undefined behavior")
        << UninitFile;
  }

  // Check for fopen-like calls that need null checking
  if (const auto *FopenCall = Result.Nodes.getNodeAs<CallExpr>("fopen_call")) {
    // Look for the parent statement to see if there's a null check
    auto Parents = Result.Context->getParents(*FopenCall);
    bool HasNullCheck = false;

    // Check if immediately used in an if/while statement condition
    if (!Parents.empty()) {
      // Direct use in if condition
      if (Parents[0].get<IfStmt>()) {
        HasNullCheck = true;
      }

      // Direct use in while condition
      if (Parents[0].get<WhileStmt>()) {
        HasNullCheck = true;
      }

      // Check if used as condition of ternary operator (not as true/false
      // branch)
      if (const auto *Cond = Parents[0].get<ConditionalOperator>()) {
        // Only consider it checked if the fopen IS the condition itself
        if (Cond->getCond()->IgnoreParenImpCasts() == FopenCall) {
          HasNullCheck = true;
        }
      }

      // Check if assigned to a variable that's later checked
      if (const auto *BinOp = Parents[0].get<BinaryOperator>()) {
        if (BinOp->getOpcode() == BO_Assign) {
          // This is an assignment; we'll track the variable usage
          // For now, we won't warn on the assignment itself
          HasNullCheck = true;
        }
      }

      // Check for var decl with initializer
      if (Parents[0].get<VarDecl>()) {
        // Variable declaration with initializer; we'll check usage later
        HasNullCheck = true;
      }

      // Check for implicit cast (e.g., in ternary true/false branch)
      // Need to look further up the tree
      if (Parents[0].get<ImplicitCastExpr>()) {
        auto GrandParents = Result.Context->getParents(Parents[0]);
        if (!GrandParents.empty()) {
          if (const auto *Cond = GrandParents[0].get<ConditionalOperator>()) {
            // If the fopen is in the condition part of the ternary
            const Expr *CondExpr = Cond->getCond()->IgnoreParenImpCasts();
            if (CondExpr == FopenCall) {
              HasNullCheck = true;
            }
          }
        }
      }
    }

    if (!HasNullCheck) {
      diag(
          FopenCall->getBeginLoc(),
          "FILE* returned from %0 may be NULL and should be checked before use")
          << FopenCall->getDirectCallee()->getName();
    }
  }

  // Check for FILE* usage after fclose
  // This is a simplified pattern-based check
  if (const auto *FcloseCall =
          Result.Nodes.getNodeAs<CallExpr>("fclose_call")) {
    if (const auto *ClosedVar = Result.Nodes.getNodeAs<VarDecl>("closed_var")) {
      // Note: Full dataflow analysis would be needed for comprehensive checking
      // This basic check just warns about the fclose call
      diag(FcloseCall->getBeginLoc(),
           "FILE* variable %0 closed here; ensure it is not used afterwards "
           "without being reassigned")
          << ClosedVar
          << FixItHint::CreateInsertion(
                 FcloseCall->getEndLoc().getLocWithOffset(1),
                 "; " + ClosedVar->getName().str() + " = nullptr");
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

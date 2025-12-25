//===--- AvoidExceptionAcrossBoundaryCheck.cpp - clang-tidy -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidExceptionAcrossBoundaryCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidExceptionAcrossBoundaryCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus)
    return;

  // Match throw expressions in any function
  Finder->addMatcher(
      cxxThrowExpr(hasAncestor(functionDecl().bind("func"))).bind("throw"),
      this);
}

void AvoidExceptionAcrossBoundaryCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Throw = Result.Nodes.getNodeAs<CXXThrowExpr>("throw");
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

  // LCOV_EXCL_START - defensive check, matcher guarantees these exist
  if (!Throw || !Func)
    return;
  // LCOV_EXCL_STOP

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Throw->getThrowLoc()))
    return;

  // Skip if function is marked noexcept (already compliant)
  const auto *FuncType = Func->getType()->getAs<FunctionProtoType>();
  if (FuncType && FuncType->canThrow() == CT_Cannot)
    return;

  // Check 1: Functions with C linkage (extern "C")
  if (Func->isExternC()) {
    diag(Throw->getThrowLoc(),
         "exception thrown in function '%0' with C linkage (extern \"C\") may "
         "cross execution boundary")
        << Func;
    diag(Func->getLocation(), "function declared here", DiagnosticIDs::Note);
    return;
  }

  // Check 2: Functions with external linkage that might cross DLL boundaries
  // This includes functions that are exported from shared libraries
  if (hasExternalLinkage(Func)) {
    // Check for explicit export attributes that suggest DLL boundary crossing
    if (Func->hasAttr<DLLExportAttr>() || Func->hasAttr<VisibilityAttr>()) {
      diag(Throw->getThrowLoc(),
           "exception thrown in exported function '%0' may cross DLL/shared "
           "library boundary")
          << Func;
      diag(Func->getLocation(), "function declared here", DiagnosticIDs::Note);
      return;
    }
  }

  // Check 3: Functions that might be callbacks to C APIs
  // These are identified by function pointer types or being passed to C
  // functions
  if (mightBeCCallback(Func)) {
    diag(Throw->getThrowLoc(),
         "exception thrown in potential callback function '%0' may cross "
         "execution boundary")
        << Func;
    diag(Func->getLocation(), "function declared here", DiagnosticIDs::Note);
    return;
  }

  // Check 4: Functions without exception specification that have external
  // linkage Functions without noexcept specification and with external linkage
  // are risky
  if (Func->hasExternalFormalLinkage() && !Func->isInlined() &&
      !Func->isStatic()) {
    // Only warn if the function is not explicitly marked as potentially
    // throwing
    if (!FuncType || FuncType->getExceptionSpecType() == EST_None) {
      diag(Throw->getThrowLoc(),
           "exception thrown in function '%0' without exception specification; "
           "may cross execution boundary if called from incompatible context")
          << Func;
      diag(Func->getLocation(),
           "consider adding noexcept specification or documenting exception "
           "behavior",
           DiagnosticIDs::Note);
    }
  }
}

bool AvoidExceptionAcrossBoundaryCheck::hasExternalLinkage(
    const FunctionDecl *Func) const {
  // Check if function has external linkage
  return Func->hasExternalFormalLinkage();
}

bool AvoidExceptionAcrossBoundaryCheck::mightBeCCallback(
    const FunctionDecl *Func) const {
  // Check if this function matches common callback patterns:
  // 1. Function pointer type with C calling convention
  // 2. Function with name suggesting callback usage (e.g., contains
  // "callback", "handler")
  // 3. Function with specific attributes suggesting callback usage

  // Check for explicit callback/handler naming patterns
  StringRef FuncName = Func->getName();
  if (FuncName.contains_insensitive("callback") ||
      FuncName.contains_insensitive("handler") ||
      FuncName.contains_insensitive("notify")) {
    return true;
  }

  // Check if function has C calling convention attributes
  if (Func->hasAttr<CDeclAttr>() || Func->hasAttr<StdCallAttr>() ||
      Func->hasAttr<FastCallAttr>()) {
    return true;
  }

  return false;
}

} // namespace clang::tidy::automotive::cpp23

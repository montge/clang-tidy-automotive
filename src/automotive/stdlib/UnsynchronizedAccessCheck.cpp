//===--- UnsynchronizedAccessCheck.cpp - clang-tidy ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UnsynchronizedAccessCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

// Check if a type is atomic
bool isAtomicType(QualType Type) {
  return Type->isAtomicType() || Type.getAsString().find("_Atomic") != std::string::npos ||
         Type.getAsString().find("atomic") != std::string::npos ||
         Type.getAsString().find("std::atomic") != std::string::npos;
}

// Check if a variable is thread-local
bool isThreadLocal(const VarDecl *VD) {
  return VD->getTLSKind() != VarDecl::TLS_None;
}

// Check if a variable is const (immutable, safe to share)
bool isEffectivelyConst(const VarDecl *VD) {
  QualType Type = VD->getType();
  return Type.isConstQualified();
}

} // namespace

void UnsynchronizedAccessCheck::registerMatchers(MatchFinder *Finder) {
  // Match references to global or static variables
  // We focus on variables with static storage duration that could be shared
  Finder->addMatcher(
      declRefExpr(
          to(varDecl(
              anyOf(hasGlobalStorage(), isStaticLocal()),
              unless(isConstexpr()),
              unless(hasType(isConstQualified()))
          ).bind("var")),
          // Only in contexts that might modify the variable
          hasAncestor(stmt(anyOf(
              binaryOperator(hasOperatorName("=")),
              unaryOperator(anyOf(hasOperatorName("++"), hasOperatorName("--"))),
              compoundStmt()
          )))
      ).bind("ref"),
      this);

  // Also match direct modifications to global/static variables
  Finder->addMatcher(
      binaryOperator(
          hasOperatorName("="),
          hasLHS(declRefExpr(to(varDecl(
              anyOf(hasGlobalStorage(), isStaticLocal()),
              unless(isConstexpr())
          ).bind("assignedVar"))))
      ).bind("assignment"),
      this);
}

void UnsynchronizedAccessCheck::check(
    const MatchFinder::MatchResult &Result) {

  const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var");
  const auto *AssignedVar = Result.Nodes.getNodeAs<VarDecl>("assignedVar");

  const VarDecl *VD = Var ? Var : AssignedVar;
  if (!VD)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
    return;

  // Skip atomic types (they have built-in synchronization)
  if (isAtomicType(VD->getType()))
    return;

  // Skip thread-local storage (each thread has its own copy)
  if (isThreadLocal(VD))
    return;

  // Skip const variables (safe to share read-only)
  if (isEffectivelyConst(VD))
    return;

  // Get the access location
  SourceLocation AccessLoc;
  if (const auto *Ref = Result.Nodes.getNodeAs<DeclRefExpr>("ref"))
    AccessLoc = Ref->getLocation();
  else if (const auto *Assign = Result.Nodes.getNodeAs<BinaryOperator>("assignment"))
    AccessLoc = Assign->getOperatorLoc();
  else
    return;

  if (Result.SourceManager->isInSystemHeader(AccessLoc))
    return;

  // Report potential unsynchronized access
  // Note: This is a heuristic warning that requires manual verification
  diag(AccessLoc,
       "potential unsynchronized access to shared variable '%0'; "
       "ensure proper synchronization (mutex, atomic operations) "
       "if accessed from multiple threads (MISRA C:2025 Rule 22.16)")
      << VD->getName();
  diag(VD->getLocation(), "variable declared here", DiagnosticIDs::Note);
}

} // namespace clang::tidy::automotive

//===--- AvoidImplicitUpcastCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidImplicitUpcastCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {
/// Check if an implicit cast is an upcast from derived to base class.
/// \param Cast The implicit cast expression.
/// \return true if this is an upcast from derived to base.
bool isImplicitUpcast(const ImplicitCastExpr *Cast) {
  if (!Cast)
    return false;

  // We're looking for DerivedToBase casts
  if (Cast->getCastKind() != CK_DerivedToBase)
    return false;

  // Get source and destination types
  QualType SourceType = Cast->getSubExpr()->getType();
  QualType DestType = Cast->getType();

  // Handle pointer types
  if (SourceType->isPointerType() && DestType->isPointerType()) {
    QualType SourcePointee = SourceType->getPointeeType();
    QualType DestPointee = DestType->getPointeeType();

    const CXXRecordDecl *SourceDecl = SourcePointee->getAsCXXRecordDecl();
    const CXXRecordDecl *DestDecl = DestPointee->getAsCXXRecordDecl();

    // Both must be class types
    if (!SourceDecl || !DestDecl)
      return false;

    // Verify it's actually an upcast (derived to base)
    return SourceDecl->isDerivedFrom(DestDecl);
  }

  // Handle reference types
  if (SourceType->isLValueReferenceType() &&
      DestType->isLValueReferenceType()) {
    QualType SourcePointee = SourceType->getPointeeType();
    QualType DestPointee = DestType->getPointeeType();

    const CXXRecordDecl *SourceDecl = SourcePointee->getAsCXXRecordDecl();
    const CXXRecordDecl *DestDecl = DestPointee->getAsCXXRecordDecl();

    // Both must be class types
    if (!SourceDecl || !DestDecl)
      return false;

    // Verify it's actually an upcast (derived to base)
    return SourceDecl->isDerivedFrom(DestDecl);
  }

  // Handle non-pointer, non-reference class types (value semantics)
  // We need to check if the source is a derived class
  const CXXRecordDecl *SourceDecl = SourceType->getAsCXXRecordDecl();
  const CXXRecordDecl *DestDecl = DestType->getAsCXXRecordDecl();

  if (!SourceDecl || !DestDecl)
    return false;

  return SourceDecl->isDerivedFrom(DestDecl);
}

/// Get a string representation of the cast type for diagnostic messages.
/// \param Type The type being cast.
/// \return "pointer" or "reference" or "value".
StringRef getCastTypeString(QualType Type) {
  if (Type->isPointerType())
    return "pointer";
  if (Type->isReferenceType())
    return "reference";
  return "value";
}

} // anonymous namespace

void AvoidImplicitUpcastCheck::registerMatchers(MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match all implicit cast expressions that might be upcasts
  Finder->addMatcher(implicitCastExpr().bind("implicit-cast"), this);
}

void AvoidImplicitUpcastCheck::check(const MatchFinder::MatchResult &Result) {

  const auto *Cast = Result.Nodes.getNodeAs<ImplicitCastExpr>("implicit-cast");
  if (!Cast)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(Cast->getBeginLoc()))
    return;

  // Skip implicit casts that are inside explicit casts (like static_cast)
  // We check if the parent is a CXXNamedCastExpr
  // Also check parent context to determine if we're dealing with references
  const auto &Parents = Result.Context->getParents(*Cast);
  const VarDecl *ParentVarDecl = nullptr;
  const ReturnStmt *ParentReturnStmt = nullptr;
  const CallExpr *ParentCallExpr = nullptr;
  if (!Parents.empty()) {
    if (Parents[0].get<CXXStaticCastExpr>() ||
        Parents[0].get<CXXDynamicCastExpr>() ||
        Parents[0].get<CXXReinterpretCastExpr>() ||
        Parents[0].get<CXXConstCastExpr>()) {
      return;
    }
    ParentVarDecl = Parents[0].get<VarDecl>();
    ParentReturnStmt = Parents[0].get<ReturnStmt>();
    ParentCallExpr = Parents[0].get<CallExpr>();
  }

  // Check if this is an implicit upcast
  if (!isImplicitUpcast(Cast))
    return;

  // Get source and destination types for the diagnostic message
  QualType SourceType = Cast->getSubExpr()->getType();
  QualType DestType = Cast->getType();

  // Extract class names for diagnostic
  const CXXRecordDecl *SourceDecl = nullptr;
  const CXXRecordDecl *DestDecl = nullptr;

  if (SourceType->isPointerType() && DestType->isPointerType()) {
    SourceDecl = SourceType->getPointeeType()->getAsCXXRecordDecl();
    DestDecl = DestType->getPointeeType()->getAsCXXRecordDecl();
  } else if (SourceType->isReferenceType() || DestType->isReferenceType()) {
    // For references, one or both might be references
    // Get the underlying class type
    SourceDecl = SourceType->getPointeeType()->getAsCXXRecordDecl();
    if (!SourceDecl)
      SourceDecl = SourceType->getAsCXXRecordDecl();

    DestDecl = DestType->getPointeeType()->getAsCXXRecordDecl();
    if (!DestDecl)
      DestDecl = DestType->getAsCXXRecordDecl();
  } else {
    SourceDecl = SourceType->getAsCXXRecordDecl();
    DestDecl = DestType->getAsCXXRecordDecl();
  }

  if (!SourceDecl || !DestDecl)
    return;

  // Determine the cast type string based on context
  // Check if we're binding to a reference by looking at parent context
  QualType TypeForDiagnostic = DestType;

  // Check VarDecl (variable initialization with reference type)
  if (ParentVarDecl && ParentVarDecl->getType()->isReferenceType()) {
    TypeForDiagnostic = ParentVarDecl->getType();
  }
  // Check ReturnStmt (returning a reference)
  else if (ParentReturnStmt) {
    // Find enclosing function to get return type
    auto ParentParents = Result.Context->getParents(*ParentReturnStmt);
    while (!ParentParents.empty()) {
      if (const FunctionDecl *FD = ParentParents[0].get<FunctionDecl>()) {
        if (FD->getReturnType()->isReferenceType()) {
          TypeForDiagnostic = FD->getReturnType();
        }
        break;
      }
      ParentParents = Result.Context->getParents(ParentParents[0]);
    }
  }
  // Check CallExpr (function argument with reference parameter)
  else if (ParentCallExpr) {
    if (const FunctionDecl *FD = ParentCallExpr->getDirectCallee()) {
      // Find which argument position this cast is at
      for (unsigned i = 0; i < ParentCallExpr->getNumArgs(); ++i) {
        if (ParentCallExpr->getArg(i) == Cast) {
          if (i < FD->getNumParams()) {
            QualType ParamType = FD->getParamDecl(i)->getType();
            if (ParamType->isReferenceType()) {
              TypeForDiagnostic = ParamType;
            }
          }
          break;
        }
      }
    }
  }

  StringRef CastTypeStr = getCastTypeString(TypeForDiagnostic);

  diag(Cast->getBeginLoc(),
       "implicit upcast from derived class %0 %1 to base class %0 %2; use "
       "static_cast for explicit conversion")
      << CastTypeStr << SourceDecl->getQualifiedNameAsString()
      << DestDecl->getQualifiedNameAsString();
}

} // namespace clang::tidy::automotive::cpp23

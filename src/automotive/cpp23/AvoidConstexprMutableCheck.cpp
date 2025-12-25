//===--- AvoidConstexprMutableCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidConstexprMutableCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidConstexprMutableCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with constexpr specifier
  // This includes both global variables and static members
  Finder->addMatcher(varDecl(isConstexpr()).bind("constexpr_var"), this);
}

bool AvoidConstexprMutableCheck::hasMutableSubObject(QualType Type) const {
  // Remove qualifiers and get the canonical type
  QualType CanonicalType = Type.getCanonicalType();

  // Handle pointer and reference types - dereference them
  if (CanonicalType->isPointerType() || CanonicalType->isReferenceType()) {
    return false; // Pointers/references themselves don't have mutable
                  // sub-objects
  }

  // Handle array types
  if (const auto *ArrayType = CanonicalType->getAsArrayTypeUnsafe()) {
    return hasMutableSubObject(ArrayType->getElementType());
  }

  // Get the underlying class/struct type
  const CXXRecordDecl *RecordDecl = CanonicalType->getAsCXXRecordDecl();
  if (!RecordDecl || !RecordDecl->hasDefinition()) {
    return false;
  }

  // Check all fields in the record
  for (const FieldDecl *Field : RecordDecl->fields()) {
    // Check if this field is mutable
    if (Field->isMutable()) {
      return true;
    }

    // Recursively check the field's type for mutable sub-objects
    if (hasMutableSubObject(Field->getType())) {
      return true;
    }
  }

  // Check all base classes
  for (const CXXBaseSpecifier &Base : RecordDecl->bases()) {
    if (hasMutableSubObject(Base.getType())) {
      return true;
    }
  }

  return false;
}

void AvoidConstexprMutableCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *VarDecl = Result.Nodes.getNodeAs<clang::VarDecl>("constexpr_var");
  if (!VarDecl)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(VarDecl->getLocation()))
    return;

  // Check if the variable's type contains mutable sub-objects
  QualType VarType = VarDecl->getType();
  if (hasMutableSubObject(VarType)) {
    diag(VarDecl->getLocation(),
         "constexpr variable '%0' should not have mutable sub-objects "
         "(MISRA C++:2023 Rule 14.1)")
        << VarDecl->getName();
  }
}

} // namespace clang::tidy::automotive::cpp23

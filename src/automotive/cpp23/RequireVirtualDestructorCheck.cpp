//===--- RequireVirtualDestructorCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RequireVirtualDestructorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void RequireVirtualDestructorCheck::registerMatchers(MatchFinder *Finder) {
  // Only match in C++ code
  if (!getLangOpts().CPlusPlus)
    return;

  // Match class definitions that:
  // 1. Have virtual functions (polymorphic), OR
  // 2. Are used as base classes (have derived classes)
  // We'll check for polymorphism and inheritance in the check() method
  Finder->addMatcher(
      cxxRecordDecl(isDefinition(), unless(isLambda())).bind("class"), this);
}

/// Check if a class is polymorphic (has virtual functions).
static bool isPolymorphic(const CXXRecordDecl *ClassDecl) {
  if (!ClassDecl || !ClassDecl->hasDefinition())
    return false;

  // Check if the class has any virtual methods
  for (const auto *Method : ClassDecl->methods()) {
    if (Method->isVirtual())
      return true;
  }

  // Check if any base class is polymorphic
  for (const auto &Base : ClassDecl->bases()) {
    const auto *BaseClass = Base.getType()->getAsCXXRecordDecl();
    if (BaseClass && isPolymorphic(BaseClass))
      return true;
  }

  return false;
}

/// Check if a class is used as a base class by examining if it has any
/// derived classes. Note: This is a simplified check that looks for
/// inheritance relationships in the current translation unit.
static bool isUsedAsBaseClass(const CXXRecordDecl *ClassDecl,
                              ASTContext *Context) {
  if (!ClassDecl || !ClassDecl->hasDefinition())
    return false;

  // Iterate through all CXXRecordDecls in the translation unit
  for (const auto *Decl : Context->getTranslationUnitDecl()->decls()) {
    if (const auto *RecordDecl = dyn_cast<CXXRecordDecl>(Decl)) {
      if (RecordDecl->hasDefinition()) {
        for (const auto &Base : RecordDecl->bases()) {
          const auto *BaseClass = Base.getType()->getAsCXXRecordDecl();
          if (BaseClass &&
              BaseClass->getCanonicalDecl() == ClassDecl->getCanonicalDecl()) {
            return true;
          }
        }
      }
    }
  }

  return false;
}

void RequireVirtualDestructorCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *ClassDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("class");

  if (!ClassDecl || !ClassDecl->hasDefinition())
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(ClassDecl->getBeginLoc()))
    return;

  // Check if this class is a candidate for the rule:
  // - Must be polymorphic (has virtual functions), OR
  // - Must be used as a base class
  bool IsPolymorphic = isPolymorphic(ClassDecl);
  bool IsBaseClass = isUsedAsBaseClass(ClassDecl, Result.Context);

  // Skip if not a polymorphic base class
  if (!IsPolymorphic && !IsBaseClass)
    return;

  // Get the destructor
  const CXXDestructorDecl *Destructor = ClassDecl->getDestructor();

  // If there's no explicit destructor, we have an implicit public non-virtual
  // destructor This is a violation for polymorphic base classes
  if (!Destructor || Destructor->isImplicit()) {
    diag(
        ClassDecl->getLocation(),
        "class '%0' is used polymorphically but has no explicit destructor; "
        "destructor should be public and virtual, or protected and non-virtual")
        << ClassDecl->getQualifiedNameAsString();
    return;
  }

  // Get destructor access level
  AccessSpecifier Access = Destructor->getAccess();
  bool IsVirtual = Destructor->isVirtual();

  // Check the two allowed patterns:
  // 1. Public and virtual - OK
  // 2. Protected and non-virtual - OK
  // Everything else is a violation

  if (Access == AS_public) {
    // Public destructor must be virtual
    if (!IsVirtual) {
      diag(Destructor->getLocation(),
           "destructor of class '%0' is public but not virtual; should be "
           "public and virtual, or protected and non-virtual")
          << ClassDecl->getQualifiedNameAsString();
    }
  } else if (Access == AS_protected) {
    // Protected destructor should be non-virtual
    if (IsVirtual) {
      diag(Destructor->getLocation(),
           "destructor of class '%0' is protected and virtual; should be "
           "public and virtual, or protected and non-virtual")
          << ClassDecl->getQualifiedNameAsString();
    }
    // Protected and non-virtual is OK - no warning
  } else if (Access == AS_private) {
    // Private destructor is generally problematic for base classes
    diag(Destructor->getLocation(),
         "destructor of class '%0' is private; should be public and virtual, "
         "or protected and non-virtual")
        << ClassDecl->getQualifiedNameAsString();
  }
}

} // namespace clang::tidy::automotive

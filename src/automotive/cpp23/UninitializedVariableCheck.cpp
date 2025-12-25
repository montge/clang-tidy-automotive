//===--- UninitializedVariableCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "UninitializedVariableCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <set>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void UninitializedVariableCheck::registerMatchers(MatchFinder *Finder) {
  // Match local variables without initialization (including static locals)
  Finder->addMatcher(varDecl(hasLocalStorage(), unless(hasInitializer(expr())),
                             unless(parmVarDecl()), unless(isImplicit()),
                             unless(isInstantiated()))
                         .bind("local_var"),
                     this);

  // Match static storage duration variables without initialization
  Finder->addMatcher(varDecl(hasGlobalStorage(), unless(hasInitializer(expr())),
                             unless(parmVarDecl()), unless(isImplicit()),
                             unless(isInstantiated()))
                         .bind("static_var"),
                     this);

  // Match constructors to check member initialization
  Finder->addMatcher(cxxConstructorDecl(isDefinition(), unless(isImplicit()),
                                        unless(isInstantiated()))
                         .bind("constructor"),
                     this);
}

void UninitializedVariableCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *LocalVar = Result.Nodes.getNodeAs<VarDecl>("local_var");
  const auto *StaticVar = Result.Nodes.getNodeAs<VarDecl>("static_var");
  const auto *Ctor = Result.Nodes.getNodeAs<CXXConstructorDecl>("constructor");

  if (LocalVar) {
    checkLocalVariable(LocalVar, Result.SourceManager);
  } else if (StaticVar) {
    checkLocalVariable(StaticVar, Result.SourceManager);
  } else if (Ctor) {
    checkConstructor(Ctor, Result.SourceManager);
  }
}

void UninitializedVariableCheck::checkLocalVariable(const VarDecl *Var,
                                                    const SourceManager *SM) {
  if (!Var)
    return;

  // Skip if in system header
  if (SM->isInSystemHeader(Var->getLocation()))
    return;

  // Skip extern declarations (they don't have definitions here)
  if (Var->hasExternalStorage())
    return;

  // Get the type
  QualType Type = Var->getType();

  // Skip reference types (compiler enforces initialization)
  if (Type->isReferenceType())
    return;

  // Skip constexpr variables (compiler enforces initialization)
  if (Var->isConstexpr())
    return;

  // Skip types with initializing constructors
  if (hasInitializingConstructor(Type))
    return;

  // Static variables without explicit initialization get zero-initialized,
  // but MISRA C++:2023 Rule 11.6.1 requires explicit initialization
  if (Var->isStaticLocal() || Var->getStorageDuration() == SD_Static ||
      Var->hasGlobalStorage()) {
    diag(Var->getLocation(),
         "static variable '%0' is not explicitly initialized; "
         "all variables shall be initialized before first use")
        << Var->getName();
    return;
  }

  // Local non-static variables
  diag(Var->getLocation(),
       "variable '%0' is not initialized at declaration; "
       "all variables shall be initialized before first use")
      << Var->getName();
}

void UninitializedVariableCheck::checkConstructor(
    const CXXConstructorDecl *Ctor, const SourceManager *SM) {
  if (!Ctor)
    return;

  // Skip if in system header
  if (SM->isInSystemHeader(Ctor->getLocation()))
    return;

  const CXXRecordDecl *ClassDecl = Ctor->getParent();
  if (!ClassDecl)
    return;

  // Build set of members initialized in constructor initializer list
  std::set<const FieldDecl *> InitializedMembers;
  for (const auto *Init : Ctor->inits()) {
    if (const FieldDecl *Member = Init->getMember()) {
      InitializedMembers.insert(Member);
    }
  }

  // Check all non-static data members
  for (const auto *Field : ClassDecl->fields()) {
    // Skip if in system header
    if (SM->isInSystemHeader(Field->getLocation()))
      continue;

    QualType Type = Field->getType();

    // Skip reference types (compiler enforces initialization)
    if (Type->isReferenceType())
      continue;

    // Skip if member has in-class initializer
    if (Field->hasInClassInitializer())
      continue;

    // Skip types with initializing constructors
    if (hasInitializingConstructor(Type))
      continue;

    // Check if member is initialized in this constructor
    if (InitializedMembers.find(Field) == InitializedMembers.end()) {
      diag(Ctor->getLocation(),
           "member variable '%0' is not initialized in constructor; "
           "all variables shall be initialized before first use")
          << Field->getName();
      diag(Field->getLocation(), "member declared here", DiagnosticIDs::Note);
    }
  }
}

bool UninitializedVariableCheck::hasInitializingConstructor(
    QualType Type) const {
  // Check if the type has a default constructor that initializes it
  if (Type->isRecordType()) {
    const CXXRecordDecl *RD = Type->getAsCXXRecordDecl();
    if (RD && RD->hasDefinition()) {
      // If the class has a user-provided default constructor or
      // a non-trivial default constructor, it will be initialized
      if (RD->hasDefaultConstructor()) {
        // Check if it's a non-trivial constructor (which does initialization)
        if (!RD->hasTrivialDefaultConstructor()) {
          return true;
        }
      }
    }
  }

  return false;
}

} // namespace clang::tidy::automotive::cpp23

//===--- RuleOfFiveCheck.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RuleOfFiveCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void RuleOfFiveCheck::registerMatchers(MatchFinder *Finder) {
  if (!getLangOpts().CPlusPlus11)
    return;

  // Match class/struct definitions
  Finder->addMatcher(cxxRecordDecl(isDefinition()).bind("class"), this);
}

void RuleOfFiveCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *ClassDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("class");
  if (!ClassDecl)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(ClassDecl->getLocation()))
    return;

  // Skip implicit/compiler-generated classes
  if (ClassDecl->isImplicit())
    return;

  // Skip templates - they're checked when instantiated
  if (ClassDecl->getDescribedClassTemplate())
    return;

  // Check which special members are user-declared
  bool HasUserDeclaredDestructor = ClassDecl->hasUserDeclaredDestructor();
  bool HasUserDeclaredCopyConstructor =
      ClassDecl->hasUserDeclaredCopyConstructor();
  bool HasUserDeclaredCopyAssignment =
      ClassDecl->hasUserDeclaredCopyAssignment();
  bool HasUserDeclaredMoveConstructor =
      ClassDecl->hasUserDeclaredMoveConstructor();
  bool HasUserDeclaredMoveAssignment =
      ClassDecl->hasUserDeclaredMoveAssignment();

  // Count how many are user-declared
  int DeclaredCount = 0;
  if (HasUserDeclaredDestructor)
    DeclaredCount++;
  if (HasUserDeclaredCopyConstructor)
    DeclaredCount++;
  if (HasUserDeclaredCopyAssignment)
    DeclaredCount++;
  if (HasUserDeclaredMoveConstructor)
    DeclaredCount++;
  if (HasUserDeclaredMoveAssignment)
    DeclaredCount++;

  // If none or all five are declared, the rule is satisfied
  if (DeclaredCount == 0 || DeclaredCount == 5)
    return;

  // Build list of missing members
  llvm::SmallVector<StringRef, 5> Missing;
  if (!HasUserDeclaredDestructor)
    Missing.push_back("destructor");
  if (!HasUserDeclaredCopyConstructor)
    Missing.push_back("copy constructor");
  if (!HasUserDeclaredCopyAssignment)
    Missing.push_back("copy assignment operator");
  if (!HasUserDeclaredMoveConstructor)
    Missing.push_back("move constructor");
  if (!HasUserDeclaredMoveAssignment)
    Missing.push_back("move assignment operator");

  // Build list of declared members
  llvm::SmallVector<StringRef, 5> Declared;
  if (HasUserDeclaredDestructor)
    Declared.push_back("destructor");
  if (HasUserDeclaredCopyConstructor)
    Declared.push_back("copy constructor");
  if (HasUserDeclaredCopyAssignment)
    Declared.push_back("copy assignment operator");
  if (HasUserDeclaredMoveConstructor)
    Declared.push_back("move constructor");
  if (HasUserDeclaredMoveAssignment)
    Declared.push_back("move assignment operator");

  // Format the message
  std::string DeclaredStr;
  for (size_t I = 0; I < Declared.size(); ++I) {
    if (I > 0) {
      if (I == Declared.size() - 1)
        DeclaredStr += " and ";
      else
        DeclaredStr += ", ";
    }
    DeclaredStr += Declared[I];
  }

  std::string MissingStr;
  for (size_t I = 0; I < Missing.size(); ++I) {
    if (I > 0) {
      if (I == Missing.size() - 1)
        MissingStr += " and ";
      else
        MissingStr += ", ";
    }
    MissingStr += Missing[I];
  }

  diag(ClassDecl->getLocation(),
       "class %0 defines %1 but is missing %2; consider defining all five "
       "special member functions (Rule of Five)")
      << ClassDecl << DeclaredStr << MissingStr;
}

} // namespace clang::tidy::automotive

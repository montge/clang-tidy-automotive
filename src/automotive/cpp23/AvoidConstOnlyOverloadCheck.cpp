//===--- AvoidConstOnlyOverloadCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidConstOnlyOverloadCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidConstOnlyOverloadCheck::registerMatchers(MatchFinder *Finder) {
  // Match all function declarations (both free functions and methods)
  Finder->addMatcher(functionDecl(unless(isImplicit())).bind("func"), this);
}

bool AvoidConstOnlyOverloadCheck::differOnlyInTopLevelQualifiers(
    QualType T1, QualType T2) const {
  // Get the unqualified types (removes top-level const/volatile)
  QualType Unqual1 = T1.getUnqualifiedType();
  QualType Unqual2 = T2.getUnqualifiedType();

  // Get the canonical types for comparison
  QualType Canon1 = Unqual1.getCanonicalType();
  QualType Canon2 = Unqual2.getCanonicalType();

  // If unqualified types are the same, check if original types differ
  if (Canon1 == Canon2) {
    // Check if the original types have different qualifiers
    Qualifiers Qual1 = T1.getQualifiers();
    Qualifiers Qual2 = T2.getQualifiers();
    return Qual1 != Qual2;
  }

  return false;
}

void AvoidConstOnlyOverloadCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");
  if (!Func)
    return;

  // Skip template instantiations and implicit declarations
  if (Func->isTemplateInstantiation() || Func->isImplicit())
    return;

  // Get the DeclContext to find other functions in the same scope
  const DeclContext *DC = Func->getDeclContext();
  if (!DC)
    return;

  // Get function name for comparison
  DeclarationName FuncName = Func->getDeclName();
  if (FuncName.isEmpty())
    return;

  // Track if we've already reported this function to avoid duplicates
  if (ReportedFunctions.count(Func))
    return;

  // Look for overloads in the same scope
  for (const auto *Decl : DC->decls()) {
    const auto *OtherFunc = dyn_cast<FunctionDecl>(Decl);
    if (!OtherFunc || OtherFunc == Func)
      continue;

    // Skip implicit declarations and template instantiations
    if (OtherFunc->isImplicit() || OtherFunc->isTemplateInstantiation())
      continue;

    // Check if they have the same name
    if (OtherFunc->getDeclName() != FuncName)
      continue;

    // Check if they have the same number of parameters
    if (Func->param_size() != OtherFunc->param_size())
      continue;

    // For methods, check if cv-qualifiers are the same
    if (const auto *Method = dyn_cast<CXXMethodDecl>(Func)) {
      if (const auto *OtherMethod = dyn_cast<CXXMethodDecl>(OtherFunc)) {
        if (Method->isConst() != OtherMethod->isConst() ||
            Method->isVolatile() != OtherMethod->isVolatile())
          continue;
      }
    }

    // Check if all parameters differ only in top-level const/volatile
    bool AllParamsDifferOnlyInQualifiers = true;
    bool AtLeastOneParamHasDifferentQualifiers = false;

    for (unsigned i = 0; i < Func->param_size(); ++i) {
      QualType T1 = Func->getParamDecl(i)->getType();
      QualType T2 = OtherFunc->getParamDecl(i)->getType();

      // Get canonical types without top-level qualifiers
      QualType Canon1 = T1.getUnqualifiedType().getCanonicalType();
      QualType Canon2 = T2.getUnqualifiedType().getCanonicalType();

      if (Canon1 != Canon2) {
        // Parameters have different types (not just qualifiers)
        AllParamsDifferOnlyInQualifiers = false;
        break;
      }

      // Check if this parameter has different top-level qualifiers
      if (T1.getCVRQualifiers() != T2.getCVRQualifiers()) {
        AtLeastOneParamHasDifferentQualifiers = true;
      }
    }

    // If all parameters match in type but at least one differs in qualifiers,
    // we have a problematic overload
    if (AllParamsDifferOnlyInQualifiers &&
        AtLeastOneParamHasDifferentQualifiers) {
      // Report only once per pair, and only on the later declaration
      if (Func->getLocation() > OtherFunc->getLocation() &&
          !ReportedFunctions.count(Func)) {
        diag(Func->getLocation(),
             "function '%0' differs from another overload only in top-level "
             "const/volatile qualification of parameters")
            << Func->getName();
        diag(OtherFunc->getLocation(), "previous declaration is here",
             DiagnosticIDs::Note);
        ReportedFunctions.insert(Func);
        ReportedFunctions.insert(OtherFunc);
      }
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

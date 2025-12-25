//===--- AvoidTemplateRefTemporaryCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTemplateRefTemporaryCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidTemplateRefTemporaryCheck::registerMatchers(MatchFinder *Finder) {
  // Match class template specializations
  Finder->addMatcher(
      classTemplateSpecializationDecl(unless(isExpansionInSystemHeader()))
          .bind("class_spec"),
      this);

  // Match function template instantiations
  Finder->addMatcher(
      functionDecl(unless(isExpansionInSystemHeader()), unless(isImplicit()))
          .bind("func_decl"),
      this);
}

bool AvoidTemplateRefTemporaryCheck::isTemporary(const Expr *E) const {
  if (!E)
    return false;

  E = E->IgnoreImpCasts();

  // Check for MaterializeTemporaryExpr
  if (isa<MaterializeTemporaryExpr>(E))
    return true;

  // Check for CXXBindTemporaryExpr
  if (isa<CXXBindTemporaryExpr>(E))
    return true;

  // Check for temporary object expressions
  if (isa<CXXTemporaryObjectExpr>(E))
    return true;

  // Check for call expressions (return temporaries)
  if (const auto *Call = dyn_cast<CallExpr>(E)) {
    QualType RetType = Call->getType();
    // Non-reference return types create temporaries
    if (!RetType->isReferenceType() && RetType->isRecordType())
      return true;
  }

  // Check for literal expressions
  if (isa<IntegerLiteral>(E) || isa<FloatingLiteral>(E) ||
      isa<CharacterLiteral>(E) || isa<CXXBoolLiteralExpr>(E))
    return true;

  // Check for CXXConstructExpr (temporary object construction)
  if (const auto *Construct = dyn_cast<CXXConstructExpr>(E)) {
    // Temporaries are typically created by prvalue expressions
    return !Construct->isElidable();
  }

  return false;
}

bool AvoidTemplateRefTemporaryCheck::templateArgumentBindsToTemporary(
    const TemplateArgument &Arg, const ASTContext &Context) const {
  if (Arg.getKind() != TemplateArgument::Declaration &&
      Arg.getKind() != TemplateArgument::Expression)
    return false;

  // Check if it's an expression argument
  if (Arg.getKind() == TemplateArgument::Expression) {
    const Expr *ArgExpr = Arg.getAsExpr();
    return isTemporary(ArgExpr);
  }

  // For declaration arguments, check if they refer to temporaries
  if (Arg.getKind() == TemplateArgument::Declaration) {
    const ValueDecl *VD = Arg.getAsDecl();
    if (const auto *Var = dyn_cast<VarDecl>(VD)) {
      // Check if it's a temporary variable (constexpr, etc.)
      if (Var->isConstexpr() && Var->hasInit()) {
        return isTemporary(Var->getInit());
      }
    }
  }

  return false;
}

void AvoidTemplateRefTemporaryCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Check class template specializations
  if (const auto *ClassSpec =
          Result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>(
              "class_spec")) {
    const TemplateArgumentList &Args = ClassSpec->getTemplateArgs();
    const ClassTemplateDecl *Template = ClassSpec->getSpecializedTemplate();
    if (!Template)
      return;

    TemplateParameterList *Params = Template->getTemplateParameters();
    if (!Params)
      return;

    for (unsigned I = 0; I < Args.size() && I < Params->size(); ++I) {
      const TemplateArgument &Arg = Args[I];

      // Check if this parameter is a non-type parameter with reference type
      if (const auto *NTTPD =
              dyn_cast<NonTypeTemplateParmDecl>(Params->getParam(I))) {
        if (NTTPD->getType()->isReferenceType()) {
          // Check if the argument binds to a temporary
          if (templateArgumentBindsToTemporary(Arg, *Result.Context)) {
            diag(ClassSpec->getLocation(),
                 "non-type template parameter of reference type binds to a "
                 "temporary object");
            diag(NTTPD->getLocation(), "template parameter declared here",
                 DiagnosticIDs::Note);
          }
        }
      }
    }
    return;
  }

  // Check function template instantiations
  if (const auto *FuncDecl =
          Result.Nodes.getNodeAs<FunctionDecl>("func_decl")) {
    // Skip if not a template instantiation
    if (FuncDecl->getTemplateSpecializationKind() == TSK_Undeclared)
      return;

    // Get the template specialization args
    const TemplateArgumentList *Args =
        FuncDecl->getTemplateSpecializationArgs();
    if (!Args)
      return;

    // Get the template declaration
    const FunctionTemplateDecl *Template = FuncDecl->getPrimaryTemplate();
    if (!Template)
      return;

    TemplateParameterList *Params = Template->getTemplateParameters();
    if (!Params)
      return;

    // Check each argument
    for (unsigned I = 0; I < Args->size() && I < Params->size(); ++I) {
      const TemplateArgument &Arg = Args->get(I);

      if (const auto *NTTPD =
              dyn_cast<NonTypeTemplateParmDecl>(Params->getParam(I))) {
        if (NTTPD->getType()->isReferenceType()) {
          if (templateArgumentBindsToTemporary(Arg, *Result.Context)) {
            diag(FuncDecl->getLocation(),
                 "non-type template parameter of reference type binds to a "
                 "temporary object");
            diag(NTTPD->getLocation(), "template parameter declared here",
                 DiagnosticIDs::Note);
          }
        }
      }
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

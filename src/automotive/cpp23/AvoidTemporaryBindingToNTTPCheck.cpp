//===--- AvoidTemporaryBindingToNTTPCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTemporaryBindingToNTTPCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidTemporaryBindingToNTTPCheck::registerMatchers(MatchFinder *Finder) {
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

bool AvoidTemporaryBindingToNTTPCheck::isTemporaryOrPRValue(
    const Expr *E) const {
  if (!E)
    return false;

  // Strip away implicit casts and parentheses
  E = E->IgnoreParenImpCasts();

  // Check for MaterializeTemporaryExpr - indicates temporary materialization
  if (isa<MaterializeTemporaryExpr>(E))
    return true;

  // Check for CXXBindTemporaryExpr - indicates binding to a temporary
  if (isa<CXXBindTemporaryExpr>(E))
    return true;

  // Check for temporary object expressions
  if (isa<CXXTemporaryObjectExpr>(E))
    return true;

  // Check for literal expressions - these are prvalues
  if (isa<IntegerLiteral>(E) || isa<FloatingLiteral>(E) ||
      isa<CharacterLiteral>(E) || isa<StringLiteral>(E) ||
      isa<CXXBoolLiteralExpr>(E) || isa<CXXNullPtrLiteralExpr>(E))
    return true;

  // Check for call expressions that return by value (creates temporary)
  if (const auto *Call = dyn_cast<CallExpr>(E)) {
    QualType RetType = Call->getType();
    // Non-reference return types create temporaries
    if (!RetType->isReferenceType())
      return true;
  }

  // Check for CXXConstructExpr - temporary object construction
  if (const auto *Construct = dyn_cast<CXXConstructExpr>(E)) {
    // Non-elidable constructs create temporaries
    if (!Construct->isElidable())
      return true;
  }

  // Check for binary/unary operators that produce prvalues
  if (isa<BinaryOperator>(E) || isa<UnaryOperator>(E))
    return true;

  // Check for compound literals
  if (isa<CompoundLiteralExpr>(E))
    return true;

  // Check expression value category
  if (E->isPRValue())
    return true;

  return false;
}

bool AvoidTemporaryBindingToNTTPCheck::bindsTemporaryToReference(
    const TemplateArgument &Arg, const NonTypeTemplateParmDecl *Param,
    const ASTContext &Context) const {

  if (!Param || !Param->getType()->isReferenceType())
    return false;

  // Check expression arguments
  if (Arg.getKind() == TemplateArgument::Expression) {
    const Expr *ArgExpr = Arg.getAsExpr();
    if (!ArgExpr)
      return false;

    // Check if the expression is a temporary or prvalue
    return isTemporaryOrPRValue(ArgExpr);
  }

  // Check declaration arguments
  if (Arg.getKind() == TemplateArgument::Declaration) {
    const ValueDecl *VD = Arg.getAsDecl();
    if (!VD)
      return false;

    // If it's a variable, check if it has static storage duration
    if (const auto *Var = dyn_cast<VarDecl>(VD)) {
      // Variables with automatic storage duration are not temporaries
      // but check if they're initialized with temporaries
      if (Var->hasLocalStorage() && !Var->isStaticLocal()) {
        // Local non-static variables could be problematic
        // but are typically caught by the compiler
        return false;
      }

      // Static, global, or constexpr variables are OK
      if (Var->hasGlobalStorage() || Var->isStaticLocal() || Var->isConstexpr())
        return false;

      // Check the initializer if present
      if (Var->hasInit()) {
        return isTemporaryOrPRValue(Var->getInit());
      }
    }

    // Parameters and other declarations are generally OK
    return false;
  }

  return false;
}

void AvoidTemporaryBindingToNTTPCheck::check(
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

    // Check each template argument against its parameter
    for (unsigned I = 0; I < Args.size() && I < Params->size(); ++I) {
      const TemplateArgument &Arg = Args[I];

      // Only check non-type template parameters
      const auto *NTTPD =
          dyn_cast<NonTypeTemplateParmDecl>(Params->getParam(I));
      if (!NTTPD)
        continue;

      // Only check reference type parameters
      if (!NTTPD->getType()->isReferenceType())
        continue;

      // Check if the argument binds a temporary to the reference
      if (bindsTemporaryToReference(Arg, NTTPD, *Result.Context)) {
        diag(ClassSpec->getLocation(),
             "non-type template parameter of reference type should not bind "
             "to a temporary object");
        diag(NTTPD->getLocation(), "reference parameter declared here",
             DiagnosticIDs::Note);
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

    // Get the template specialization arguments
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

      const auto *NTTPD =
          dyn_cast<NonTypeTemplateParmDecl>(Params->getParam(I));
      if (!NTTPD || !NTTPD->getType()->isReferenceType())
        continue;

      if (bindsTemporaryToReference(Arg, NTTPD, *Result.Context)) {
        diag(FuncDecl->getLocation(),
             "non-type template parameter of reference type should not bind "
             "to a temporary object");
        diag(NTTPD->getLocation(), "reference parameter declared here",
             DiagnosticIDs::Note);
      }
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

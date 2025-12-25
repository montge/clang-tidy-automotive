//===--- AvoidUnusedVariadicTemplateCheck.cpp - clang-tidy ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUnusedVariadicTemplateCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

namespace {
// Check if a template parameter pack is in the parameter list
bool hasParameterPack(const TemplateParameterList *TPL) {
  if (!TPL)
    return false;

  for (const auto *Param : *TPL) {
    if (const auto *TTPD = dyn_cast<TemplateTypeParmDecl>(Param)) {
      if (TTPD->isParameterPack())
        return true;
    } else if (const auto *NTTPD = dyn_cast<NonTypeTemplateParmDecl>(Param)) {
      if (NTTPD->isParameterPack())
        return true;
    } else if (const auto *TTPD = dyn_cast<TemplateTemplateParmDecl>(Param)) {
      if (TTPD->isParameterPack())
        return true;
    }
  }
  return false;
}

// Count the number of arguments in the last pack expansion
// Returns the size of the pack that was expanded, or 0 if not a pack
unsigned getPackExpansionSize(const TemplateArgumentList *Args) {
  if (!Args || Args->size() == 0)
    return 0;

  // Look through template arguments to find pack expansions
  // The last arguments in the list will be the expanded pack elements
  unsigned PackSize = 0;
  for (unsigned I = 0; I < Args->size(); ++I) {
    const TemplateArgument &Arg = Args->get(I);
    if (Arg.getKind() == TemplateArgument::Pack) {
      PackSize = Arg.pack_size();
    }
  }

  return PackSize;
}
} // namespace

bool AvoidUnusedVariadicTemplateCheck::hasVariadicParameter(
    const TemplateParameterList *TPL) const {
  return hasParameterPack(TPL);
}

unsigned AvoidUnusedVariadicTemplateCheck::countVariadicArgs(
    const TemplateArgumentList *Args) const {
  if (!Args)
    return 0;

  // Check if this is an explicit instantiation with pack expansion
  unsigned PackSize = getPackExpansionSize(Args);
  if (PackSize > 0)
    return PackSize;

  // For implicit instantiations, we need to count the actual arguments
  // If we have more arguments than expected, the extras are from pack expansion
  // This is a heuristic: count all arguments
  return Args->size();
}

void AvoidUnusedVariadicTemplateCheck::registerMatchers(MatchFinder *Finder) {
  // Match function template declarations with parameter packs
  Finder->addMatcher(functionTemplateDecl(unless(isExpansionInSystemHeader()),
                                          unless(isImplicit()))
                         .bind("funcTemplate"),
                     this);

  // Match class template declarations with parameter packs
  Finder->addMatcher(classTemplateDecl(unless(isExpansionInSystemHeader()),
                                       unless(isImplicit()))
                         .bind("classTemplate"),
                     this);

  // Match function template specializations (instantiations)
  // Use hasTemplateSpecializationKind to match template instantiations
  Finder->addMatcher(
      functionDecl(unless(isExpansionInSystemHeader()), unless(isImplicit()))
          .bind("funcInstantiation"),
      this);

  // Match class template specializations (instantiations)
  Finder->addMatcher(
      classTemplateSpecializationDecl(unless(isExpansionInSystemHeader()))
          .bind("classInstantiation"),
      this);
}

void AvoidUnusedVariadicTemplateCheck::check(
    const MatchFinder::MatchResult &Result) {

  // Handle function template declarations
  if (const auto *FTD =
          Result.Nodes.getNodeAs<FunctionTemplateDecl>("funcTemplate")) {
    const auto *TPL = FTD->getTemplateParameters();
    if (hasVariadicParameter(TPL)) {
      // Record this variadic template
      TemplateInfo &Info = VariadicTemplates[FTD];
      Info.Loc = FTD->getLocation();
      Info.Name = FTD->getNameAsString();
    }
    return;
  }

  // Handle class template declarations
  if (const auto *CTD =
          Result.Nodes.getNodeAs<ClassTemplateDecl>("classTemplate")) {
    const auto *TPL = CTD->getTemplateParameters();
    if (hasVariadicParameter(TPL)) {
      // Record this variadic template
      TemplateInfo &Info = VariadicTemplates[CTD];
      Info.Loc = CTD->getLocation();
      Info.Name = CTD->getNameAsString();
    }
    return;
  }

  // Handle function template instantiations
  if (const auto *FD =
          Result.Nodes.getNodeAs<FunctionDecl>("funcInstantiation")) {
    // Skip if not a template instantiation
    if (FD->getTemplateSpecializationKind() == TSK_Undeclared)
      return;

    if (const auto *FTD = FD->getPrimaryTemplate()) {
      auto It = VariadicTemplates.find(FTD);
      if (It != VariadicTemplates.end()) {
        It->second.HasAnyInstantiation = true;

        // Check if this instantiation uses variadic arguments
        if (const auto *TSI = FD->getTemplateSpecializationArgs()) {
          // Count how many arguments came from the parameter pack
          // Strategy: Check the template parameters vs arguments
          const auto *TPL = FTD->getTemplateParameters();
          if (TPL) {
            unsigned NumFixedParams = 0;
            bool HasPack = false;

            // Count fixed parameters and detect pack
            for (const auto *Param : *TPL) {
              if (const auto *TTPD = dyn_cast<TemplateTypeParmDecl>(Param)) {
                if (TTPD->isParameterPack()) {
                  HasPack = true;
                  break;
                }
                NumFixedParams++;
              } else if (const auto *NTTPD =
                             dyn_cast<NonTypeTemplateParmDecl>(Param)) {
                if (NTTPD->isParameterPack()) {
                  HasPack = true;
                  break;
                }
                NumFixedParams++;
              }
            }

            // If we have a pack and more args than fixed params, pack was
            // expanded
            if (HasPack && TSI->size() > NumFixedParams) {
              It->second.HasVariadicInstantiation = true;
            }
          }
        }
      }
    }
    return;
  }

  // Handle class template instantiations
  if (const auto *CTSD =
          Result.Nodes.getNodeAs<ClassTemplateSpecializationDecl>(
              "classInstantiation")) {
    if (const auto *CTD = CTSD->getSpecializedTemplate()) {
      auto It = VariadicTemplates.find(CTD);
      if (It != VariadicTemplates.end()) {
        It->second.HasAnyInstantiation = true;

        // Check if this instantiation uses variadic arguments
        const auto &Args = CTSD->getTemplateArgs();
        const auto *TPL = CTD->getTemplateParameters();

        if (TPL) {
          unsigned NumFixedParams = 0;
          bool HasPack = false;

          // Count fixed parameters and detect pack
          for (const auto *Param : *TPL) {
            if (const auto *TTPD = dyn_cast<TemplateTypeParmDecl>(Param)) {
              if (TTPD->isParameterPack()) {
                HasPack = true;
                break;
              }
              NumFixedParams++;
            } else if (const auto *NTTPD =
                           dyn_cast<NonTypeTemplateParmDecl>(Param)) {
              if (NTTPD->isParameterPack()) {
                HasPack = true;
                break;
              }
              NumFixedParams++;
            }
          }

          // If we have a pack and more args than fixed params, pack was
          // expanded
          if (HasPack && Args.size() > NumFixedParams) {
            It->second.HasVariadicInstantiation = true;
          }
        }
      }
    }
    return;
  }
}

void AvoidUnusedVariadicTemplateCheck::onEndOfTranslationUnit() {
  // Report variadic templates that were instantiated but never with variadic
  // args
  for (const auto &Entry : VariadicTemplates) {
    const TemplateInfo &Info = Entry.second;

    // Only warn if template was instantiated at least once
    // but never with variadic arguments
    if (Info.HasAnyInstantiation && !Info.HasVariadicInstantiation) {
      diag(Info.Loc,
           "variadic template '%0' is never instantiated with variadic "
           "arguments; consider using a regular template instead")
          << Info.Name;
    }
  }
}

} // namespace clang::tidy::automotive::cpp23

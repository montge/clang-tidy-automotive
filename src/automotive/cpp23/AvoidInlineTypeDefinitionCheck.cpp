//===--- AvoidInlineTypeDefinitionCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidInlineTypeDefinitionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

void AvoidInlineTypeDefinitionCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with inline type definitions
  Finder->addMatcher(varDecl(unless(isExpansionInSystemHeader())).bind("var"),
                     this);

  // Match function declarations with inline type definitions in parameters
  // Exclude implicit functions (constructors, destructors generated for
  // anonymous types)
  Finder->addMatcher(
      functionDecl(unless(isExpansionInSystemHeader()), unless(isImplicit()))
          .bind("func"),
      this);

  // Match field declarations with inline type definitions
  Finder->addMatcher(
      fieldDecl(unless(isExpansionInSystemHeader())).bind("field"), this);

  // Match typedef/using declarations with inline type definitions
  Finder->addMatcher(
      typedefDecl(unless(isExpansionInSystemHeader())).bind("typedef"), this);

  // Match type alias declarations
  Finder->addMatcher(
      typeAliasDecl(unless(isExpansionInSystemHeader())).bind("alias"), this);
}

/// Get the TagDecl from a type if it's an inline definition
static const TagDecl *getInlineTagDecl(QualType QT) {
  // Strip qualifiers and get the canonical type
  QT = QT.getCanonicalType();

  // Handle pointer and reference types
  while (QT->isPointerType() || QT->isReferenceType()) {
    QT = QT->getPointeeType().getCanonicalType();
  }

  // Handle array types
  while (const auto *AT = dyn_cast<ArrayType>(QT.getTypePtr())) {
    QT = AT->getElementType().getCanonicalType();
  }

  // Get the tag type
  if (const auto *TT = QT->getAs<TagType>()) {
    const TagDecl *TD = TT->getDecl();
    // Check if this is an anonymous definition
    if (TD && TD->isThisDeclarationADefinition() && !TD->getIdentifier()) {
      return TD;
    }
  }

  return nullptr;
}

void AvoidInlineTypeDefinitionCheck::check(
    const MatchFinder::MatchResult &Result) {
  if (const auto *Var = Result.Nodes.getNodeAs<VarDecl>("var")) {
    if (const TagDecl *TD = getInlineTagDecl(Var->getType())) {
      diag(TD->getLocation(),
           "type defined inline in variable declaration; define the type "
           "separately")
          << Var->getName();
    }
    return;
  }

  if (const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    // Check return type
    if (const TagDecl *TD = getInlineTagDecl(Func->getReturnType())) {
      diag(TD->getLocation(),
           "type defined inline in function return type; define the type "
           "separately");
    }

    // Check parameter types
    for (const ParmVarDecl *Param : Func->parameters()) {
      if (const TagDecl *TD = getInlineTagDecl(Param->getType())) {
        diag(TD->getLocation(),
             "type defined inline in function parameter; define the type "
             "separately");
      }
    }
    return;
  }

  if (const auto *Field = Result.Nodes.getNodeAs<FieldDecl>("field")) {
    if (const TagDecl *TD = getInlineTagDecl(Field->getType())) {
      diag(TD->getLocation(),
           "type defined inline in field declaration; define the type "
           "separately")
          << Field->getName();
    }
    return;
  }

  if (const auto *TD = Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
    if (const TagDecl *Tag = getInlineTagDecl(TD->getUnderlyingType())) {
      diag(Tag->getLocation(),
           "type defined inline in typedef; define the type separately");
    }
    return;
  }

  if (const auto *Alias = Result.Nodes.getNodeAs<TypeAliasDecl>("alias")) {
    if (const TagDecl *Tag = getInlineTagDecl(Alias->getUnderlyingType())) {
      diag(Tag->getLocation(),
           "type defined inline in type alias; define the type separately");
    }
    return;
  }
}

} // namespace clang::tidy::automotive::cpp23

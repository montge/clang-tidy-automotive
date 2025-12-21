//===--- ImplicitIntCheck.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ImplicitIntCheck.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

void ImplicitIntCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations that might have implicit int
  Finder->addMatcher(varDecl().bind("var"), this);
  // Match function declarations that might have implicit int return type
  Finder->addMatcher(functionDecl().bind("func"), this);
}

void ImplicitIntCheck::check(const MatchFinder::MatchResult &Result) {
  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    checkVarDecl(VD, *Result.SourceManager, Result.Context->getLangOpts());
  }
  if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    checkFuncDecl(FD, *Result.SourceManager, Result.Context->getLangOpts());
  }
}

void ImplicitIntCheck::checkVarDecl(const VarDecl *VD, const SourceManager &SM,
                                    const LangOptions &LangOpts) {
  // Skip if in system header or has explicit type written
  if (SM.isInSystemHeader(VD->getLocation()))
    return;

  // Check if the type specifier location is valid - if invalid, type was implicit
  TypeSourceInfo *TSI = VD->getTypeSourceInfo();
  if (!TSI)
    return;

  TypeLoc TL = TSI->getTypeLoc();

  // Get the source range for the type
  SourceRange TypeRange = TL.getSourceRange();
  if (!TypeRange.isValid())
    return;

  // Get the text of the declaration before the variable name
  SourceLocation DeclStart = VD->getBeginLoc();
  SourceLocation NameLoc = VD->getLocation();

  if (!DeclStart.isValid() || !NameLoc.isValid())
    return;

  // Check if the declaration has an explicit type keyword
  CharSourceRange BeforeNameRange = CharSourceRange::getCharRange(DeclStart, NameLoc);
  StringRef DeclText = Lexer::getSourceText(BeforeNameRange, SM, LangOpts);

  // Look for type keywords - if none found and it's an int type, it's implicit
  QualType QT = VD->getType();
  if (!QT->isIntegerType())
    return;

  // Check if there's an explicit type specifier
  // These are the keywords that indicate explicit int type
  bool hasExplicitType = DeclText.contains("int") ||
                         DeclText.contains("char") ||
                         DeclText.contains("short") ||
                         DeclText.contains("long") ||
                         DeclText.contains("_Bool") ||
                         DeclText.contains("bool");

  if (!hasExplicitType && QT->isIntegerType()) {
    // Check for storage class or qualifiers without type
    bool hasStorageOrQualifier = DeclText.contains("const") ||
                                  DeclText.contains("volatile") ||
                                  DeclText.contains("static") ||
                                  DeclText.contains("extern") ||
                                  DeclText.contains("register") ||
                                  DeclText.contains("unsigned") ||
                                  DeclText.contains("signed");

    if (hasStorageOrQualifier) {
      diag(VD->getLocation(), "implicit int type in declaration");
    }
  }
}

void ImplicitIntCheck::checkFuncDecl(const FunctionDecl *FD, const SourceManager &SM,
                                     const LangOptions &LangOpts) {
  // Skip if in system header
  if (SM.isInSystemHeader(FD->getLocation()))
    return;

  // Skip if it's a definition that was already declared with explicit type
  if (FD->isThisDeclarationADefinition() && FD->getPreviousDecl())
    return;

  // Get the source range for the function
  SourceLocation FuncStart = FD->getBeginLoc();
  SourceLocation NameLoc = FD->getLocation();

  if (!FuncStart.isValid() || !NameLoc.isValid())
    return;

  // Get the text before the function name
  CharSourceRange BeforeNameRange = CharSourceRange::getCharRange(FuncStart, NameLoc);
  StringRef DeclText = Lexer::getSourceText(BeforeNameRange, SM, LangOpts);

  // Check if there's an explicit return type
  QualType ReturnType = FD->getReturnType();
  if (ReturnType->isIntegerType() && !ReturnType->isVoidType()) {
    bool hasExplicitType = DeclText.contains("int") ||
                           DeclText.contains("char") ||
                           DeclText.contains("short") ||
                           DeclText.contains("long") ||
                           DeclText.contains("void") ||
                           DeclText.contains("_Bool") ||
                           DeclText.contains("bool");

    if (!hasExplicitType) {
      diag(FD->getLocation(), "implicit int return type in function declaration");
    }
  }
}

} // namespace clang::tidy::automotive

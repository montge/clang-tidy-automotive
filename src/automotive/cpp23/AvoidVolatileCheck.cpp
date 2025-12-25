//===--- AvoidVolatileCheck.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidVolatileCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive::cpp23 {

AvoidVolatileCheck::AvoidVolatileCheck(StringRef Name,
                                       ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context) {
  // Parse the semicolon-separated list of allowed namespaces
  StringRef NamespacesOption =
      Options.get("AllowedNamespaces", "hardware;hw;mmio");
  SmallVector<StringRef, 4> Namespaces;
  NamespacesOption.split(Namespaces, ';', -1, false);

  for (StringRef NS : Namespaces) {
    StringRef Trimmed = NS.trim();
    if (!Trimmed.empty())
      AllowedNamespaces.push_back(Trimmed.str());
  }
}

void AvoidVolatileCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  // Join the allowed namespaces back into a semicolon-separated string
  std::string JoinedNamespaces;
  for (size_t I = 0; I < AllowedNamespaces.size(); ++I) {
    if (I > 0)
      JoinedNamespaces += ";";
    JoinedNamespaces += AllowedNamespaces[I];
  }
  Options.store(Opts, "AllowedNamespaces", JoinedNamespaces);
}

bool AvoidVolatileCheck::isInAllowedNamespace(const Decl *D) const {
  if (AllowedNamespaces.empty())
    return false;

  const DeclContext *DC = D->getDeclContext();
  while (DC) {
    if (const auto *ND = dyn_cast<NamespaceDecl>(DC)) {
      StringRef NSName = ND->getName();
      for (const auto &AllowedNS : AllowedNamespaces) {
        if (NSName == AllowedNS)
          return true;
      }
    }
    DC = DC->getParent();
  }
  return false;
}

void AvoidVolatileCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with volatile type
  Finder->addMatcher(
      varDecl(hasType(isVolatileQualified())).bind("volatile_var"), this);

  // Match function parameters with volatile type
  Finder->addMatcher(
      parmVarDecl(hasType(isVolatileQualified())).bind("volatile_param"), this);

  // Match function declarations with volatile return type
  Finder->addMatcher(
      functionDecl(returns(isVolatileQualified())).bind("volatile_return"),
      this);

  // Match field declarations with volatile type
  Finder->addMatcher(
      fieldDecl(hasType(isVolatileQualified())).bind("volatile_field"), this);
}

void AvoidVolatileCheck::check(const MatchFinder::MatchResult &Result) {
  const Decl *D = nullptr;
  StringRef DeclType;

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("volatile_var")) {
    D = VD;
    DeclType = "variable";
  } else if (const auto *PVD =
                 Result.Nodes.getNodeAs<ParmVarDecl>("volatile_param")) {
    D = PVD;
    DeclType = "parameter";
  } else if (const auto *FD =
                 Result.Nodes.getNodeAs<FunctionDecl>("volatile_return")) {
    D = FD;
    DeclType = "return type";
  } else if (const auto *FLD =
                 Result.Nodes.getNodeAs<FieldDecl>("volatile_field")) {
    D = FLD;
    DeclType = "field";
  }

  if (!D)
    return;

  // Skip system headers
  if (Result.SourceManager->isInSystemHeader(D->getLocation()))
    return;

  // Check if declaration is in an allowed namespace
  if (isInAllowedNamespace(D))
    return;

  diag(D->getLocation(),
       "the volatile keyword shall only be used for hardware access; "
       "volatile %0 declared outside allowed namespaces (MISRA C++:2023 Rule "
       "10.4.1)")
      << DeclType;
}

} // namespace clang::tidy::automotive::cpp23

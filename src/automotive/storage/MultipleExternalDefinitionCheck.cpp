//===--- MultipleExternalDefinitionCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MultipleExternalDefinitionCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/MemoryBuffer.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

MultipleExternalDefinitionCheck::MultipleExternalDefinitionCheck(
    StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      SymbolDatabase(Options.get("SymbolDatabase", "external_symbols.json")),
      MaxLocations(Options.get("MaxLocations", 10U)) {}

void MultipleExternalDefinitionCheck::storeOptions(
    ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "SymbolDatabase", SymbolDatabase);
  Options.store(Opts, "MaxLocations", MaxLocations);
}

std::string
MultipleExternalDefinitionCheck::getLocationKey(const SymbolInfo &Info) {
  return Info.File + ":" + std::to_string(Info.Line) + ":" +
         std::to_string(Info.Column);
}

void MultipleExternalDefinitionCheck::loadSymbolDatabase() {
  if (DatabaseLoaded)
    return;
  DatabaseLoaded = true;

  auto BufferOrErr = llvm::MemoryBuffer::getFile(SymbolDatabase);
  if (!BufferOrErr) {
    // Silently continue - the check requires a pre-collected symbol database.
    // Users should run automotive-collect-external-symbols first.
    return;
  }

  auto Buffer = std::move(*BufferOrErr);
  auto JsonOrErr = llvm::json::parse(Buffer->getBuffer());
  if (!JsonOrErr) {
    // Silently continue on invalid JSON
    return;
  }

  auto *Root = JsonOrErr->getAsObject();
  if (!Root)
    return;

  auto *Symbols = Root->getArray("symbols");
  if (!Symbols)
    return;

  for (const auto &SymVal : *Symbols) {
    auto *SymObj = SymVal.getAsObject();
    if (!SymObj)
      continue;

    auto Name = SymObj->getString("name");
    if (!Name || Name->empty())
      continue;

    SymbolInfo Info;
    if (auto File = SymObj->getString("file"))
      Info.File = File->str();
    if (auto Line = SymObj->getInteger("line"))
      Info.Line = *Line;
    if (auto Column = SymObj->getInteger("column"))
      Info.Column = *Column;
    if (auto Kind = SymObj->getString("kind"))
      Info.Kind = Kind->str();
    if (auto Type = SymObj->getString("type"))
      Info.Type = Type->str();
    if (auto IsDef = SymObj->getBoolean("isDefinition"))
      Info.IsDefinition = *IsDef;

    ExternalSymbols[*Name].push_back(Info);
  }
}

void MultipleExternalDefinitionCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable definitions with external linkage
  Finder->addMatcher(varDecl(hasExternalFormalLinkage(), isDefinition(),
                             unless(isExpansionInSystemHeader()))
                         .bind("var"),
                     this);

  // Match function definitions with external linkage
  Finder->addMatcher(functionDecl(hasExternalFormalLinkage(), isDefinition(),
                                  unless(isExpansionInSystemHeader()))
                         .bind("func"),
                     this);
}

void MultipleExternalDefinitionCheck::check(
    const MatchFinder::MatchResult &Result) {
  // Load database on first check
  loadSymbolDatabase();

  std::string Name;
  std::string Kind;
  SourceLocation Loc;
  std::string CurrentFile;

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;
    // Only check definitions
    if (!VD->isThisDeclarationADefinition())
      return;
    Name = VD->getNameAsString();
    Kind = "variable";
    Loc = VD->getLocation();
    CurrentFile = Result.SourceManager->getFilename(Loc).str();
  } else if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
      return;
    // Only check definitions
    if (!FD->isThisDeclarationADefinition())
      return;
    if (FD->isMain())
      return;
    Name = FD->getNameAsString();
    Kind = "function";
    Loc = FD->getLocation();
    CurrentFile = Result.SourceManager->getFilename(Loc).str();
  } else {
    return;
  }

  if (Name.empty() || CurrentFile.empty())
    return;

  // Look up this symbol in the database
  auto It = ExternalSymbols.find(Name);
  if (It == ExternalSymbols.end())
    return;

  const auto &Occurrences = It->second;

  // Find DEFINITIONS in OTHER files (not the current file)
  std::vector<const SymbolInfo *> OtherFileDefinitions;
  for (const auto &Occ : Occurrences) {
    if (Occ.File != CurrentFile && Occ.IsDefinition) {
      OtherFileDefinitions.push_back(&Occ);
    }
  }

  // If there are definitions in other files, report a violation
  if (!OtherFileDefinitions.empty()) {
    // Deduplicate locations using a set
    std::set<std::string> UniqueLocationKeys;
    std::vector<std::string> UniqueLocations;
    for (const auto *Occ : OtherFileDefinitions) {
      std::string Key = getLocationKey(*Occ);
      if (UniqueLocationKeys.insert(Key).second) {
        UniqueLocations.push_back(Key);
      }
    }

    // Build list of locations for the message
    std::string OtherLocations;
    size_t NumToShow =
        (MaxLocations > 0 && UniqueLocations.size() > MaxLocations)
            ? MaxLocations
            : UniqueLocations.size();
    for (size_t I = 0; I < NumToShow; ++I) {
      if (I > 0)
        OtherLocations += ", ";
      OtherLocations += UniqueLocations[I];
    }
    if (MaxLocations > 0 && UniqueLocations.size() > MaxLocations) {
      OtherLocations +=
          " ... and " +
          std::to_string(UniqueLocations.size() - MaxLocations) +
          " more location(s)";
    }

    diag(Loc, "external %0 '%1' has multiple definitions; also defined at %2")
        << Kind << Name << OtherLocations;
  }
}

} // namespace clang::tidy::automotive

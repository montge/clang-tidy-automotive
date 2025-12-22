//===--- ExternalSymbolCollectorCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExternalSymbolCollectorCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

ExternalSymbolCollectorCheck::ExternalSymbolCollectorCheck(
    StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      OutputFile(Options.get("OutputFile", "external_symbols.json")) {
  // Load existing symbols if file exists (for multi-file runs)
  loadExistingSymbols();
}

ExternalSymbolCollectorCheck::~ExternalSymbolCollectorCheck() = default;

void ExternalSymbolCollectorCheck::storeOptions(
    ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "OutputFile", OutputFile);
}

void ExternalSymbolCollectorCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations with external linkage
  Finder->addMatcher(
      varDecl(hasExternalFormalLinkage(), unless(isExpansionInSystemHeader()))
          .bind("var"),
      this);

  // Match function declarations with external linkage
  Finder->addMatcher(functionDecl(hasExternalFormalLinkage(),
                                  unless(isExpansionInSystemHeader()))
                         .bind("func"),
                     this);
}

void ExternalSymbolCollectorCheck::check(
    const MatchFinder::MatchResult &Result) {
  ExternalSymbol Symbol;

  if (const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(VD->getLocation()))
      return;

    Symbol.Name = VD->getNameAsString();
    Symbol.Kind = "variable";
    Symbol.IsDefinition = VD->isThisDeclarationADefinition();
    Symbol.Type = VD->getType().getAsString();

    SourceLocation Loc = VD->getLocation();
    Symbol.File = Result.SourceManager->getFilename(Loc).str();
    Symbol.Line = Result.SourceManager->getSpellingLineNumber(Loc);
    Symbol.Column = Result.SourceManager->getSpellingColumnNumber(Loc);
  } else if (const auto *FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
    // Skip if in system header
    if (Result.SourceManager->isInSystemHeader(FD->getLocation()))
      return;

    // Skip main function
    if (FD->isMain())
      return;

    Symbol.Name = FD->getNameAsString();
    Symbol.Kind = "function";
    Symbol.IsDefinition = FD->isThisDeclarationADefinition();
    Symbol.Type = FD->getType().getAsString();

    SourceLocation Loc = FD->getLocation();
    Symbol.File = Result.SourceManager->getFilename(Loc).str();
    Symbol.Line = Result.SourceManager->getSpellingLineNumber(Loc);
    Symbol.Column = Result.SourceManager->getSpellingColumnNumber(Loc);
  } else {
    return;
  }

  // Don't add empty names or files
  if (Symbol.Name.empty() || Symbol.File.empty())
    return;

  CollectedSymbols.push_back(Symbol);
}

void ExternalSymbolCollectorCheck::onEndOfTranslationUnit() {
  writeSymbolsToFile();
}

void ExternalSymbolCollectorCheck::loadExistingSymbols() {
  auto BufferOrErr = llvm::MemoryBuffer::getFile(OutputFile);
  if (!BufferOrErr)
    return; // File doesn't exist yet, that's fine

  auto Buffer = std::move(*BufferOrErr);
  auto JsonOrErr = llvm::json::parse(Buffer->getBuffer());
  if (!JsonOrErr)
    return; // Invalid JSON, start fresh

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

    ExternalSymbol Symbol;
    if (auto Name = SymObj->getString("name"))
      Symbol.Name = Name->str();
    if (auto File = SymObj->getString("file"))
      Symbol.File = File->str();
    if (auto Line = SymObj->getInteger("line"))
      Symbol.Line = *Line;
    if (auto Column = SymObj->getInteger("column"))
      Symbol.Column = *Column;
    if (auto Kind = SymObj->getString("kind"))
      Symbol.Kind = Kind->str();
    if (auto IsDef = SymObj->getBoolean("isDefinition"))
      Symbol.IsDefinition = *IsDef;
    if (auto Type = SymObj->getString("type"))
      Symbol.Type = Type->str();

    if (!Symbol.Name.empty() && !Symbol.File.empty())
      CollectedSymbols.push_back(Symbol);
  }
}

void ExternalSymbolCollectorCheck::writeSymbolsToFile() {
  if (CollectedSymbols.empty())
    return;

  llvm::json::Array SymbolsArray;
  for (const auto &Sym : CollectedSymbols) {
    llvm::json::Object SymObj;
    SymObj["name"] = Sym.Name;
    SymObj["file"] = Sym.File;
    SymObj["line"] = static_cast<int64_t>(Sym.Line);
    SymObj["column"] = static_cast<int64_t>(Sym.Column);
    SymObj["kind"] = Sym.Kind;
    SymObj["isDefinition"] = Sym.IsDefinition;
    SymObj["type"] = Sym.Type;
    SymbolsArray.push_back(std::move(SymObj));
  }

  llvm::json::Object Root;
  Root["version"] = 1;
  Root["symbols"] = std::move(SymbolsArray);

  std::error_code EC;
  llvm::raw_fd_ostream OS(OutputFile, EC, llvm::sys::fs::OF_Text);
  if (EC) {
    llvm::errs() << "Error writing symbol database: " << EC.message() << "\n";
    return;
  }

  OS << llvm::json::Value(std::move(Root));
}

} // namespace clang::tidy::automotive

//===--- ExternalSymbolCollectorCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXTERNALSYMBOLCOLLECTORCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXTERNALSYMBOLCOLLECTORCHECK_H

#include "../../ClangTidyCheck.h"
#include "llvm/Support/JSON.h"
#include <string>
#include <vector>

namespace clang::tidy::automotive {

/// Collects external symbols for cross-translation-unit analysis.
///
/// This check is the first pass of a two-pass approach for detecting
/// MISRA C:2012 Rule 5.8 (unique external identifiers) and Rule 8.6
/// (single external definition).
///
/// Pass 1: Run this check to collect all external symbols to a JSON file.
/// Pass 2: Run the duplicate checker with the collected database.
///
/// Usage:
/// \code
///   # Pass 1: Collect symbols
///   clang-tidy --checks='automotive-collect-external-symbols' \
///       -config='{CheckOptions:
///       {automotive-collect-external-symbols.OutputFile: "symbols.json"}}' \
///       file1.c file2.c --
///
///   # Pass 2: Check for duplicates
///   clang-tidy --checks='automotive-c23-req-5.8' \
///       -config='{CheckOptions: {automotive-c23-req-5.8.SymbolDatabase:
///       "symbols.json"}}' \ file1.c file2.c --
/// \endcode
class ExternalSymbolCollectorCheck : public ClangTidyCheck {
public:
  ExternalSymbolCollectorCheck(StringRef Name, ClangTidyContext *Context);
  ~ExternalSymbolCollectorCheck() override;

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void onEndOfTranslationUnit() override;

  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

  /// Structure to hold information about an external symbol.
  struct ExternalSymbol {
    std::string Name;
    std::string File;
    unsigned Line;
    unsigned Column;
    std::string Kind;  // "variable" or "function"
    bool IsDefinition; // true if this is a definition, false if declaration
    std::string Type;  // The type as a string
  };

private:
  std::string OutputFile;
  std::vector<ExternalSymbol> CollectedSymbols;

  /// Writes collected symbols to JSON file.
  void writeSymbolsToFile();

  /// Loads existing symbols from file (for appending across multiple runs).
  void loadExistingSymbols();
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_EXTERNALSYMBOLCOLLECTORCHECK_H

//===--- DuplicateExternalIdentifierCheck.h - clang-tidy --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATEEXTERNALIDENTIFIERCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATEEXTERNALIDENTIFIERCHECK_H

#include "../../ClangTidyCheck.h"
#include "llvm/ADT/StringMap.h"
#include <set>
#include <string>
#include <vector>

namespace clang::tidy::automotive {

/// Detects duplicate external identifiers across translation units.
///
/// MISRA C:2012 Rule 5.8: Identifiers that define objects or functions with
/// external linkage shall be unique.
///
/// This is the second pass of a two-pass CTU analysis:
/// 1. First run automotive-collect-external-symbols to build the database
/// 2. Then run this check with the SymbolDatabase option set
///
/// Usage:
/// \code
///   clang-tidy --checks='automotive-c23-req-5.8' \
///       -config='{CheckOptions: {automotive-c23-req-5.8.SymbolDatabase:
///       "symbols.json"}}' \ file1.c file2.c --
/// \endcode
class DuplicateExternalIdentifierCheck : public ClangTidyCheck {
public:
  DuplicateExternalIdentifierCheck(StringRef Name, ClangTidyContext *Context);

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;

private:
  struct SymbolInfo {
    std::string File;
    unsigned Line;
    unsigned Column;
    std::string Kind;
    std::string Type;
    bool IsDefinition = false;
  };

  std::string SymbolDatabase;
  /// Maximum number of locations to include in diagnostic (0 = unlimited).
  unsigned MaxLocations;
  llvm::StringMap<std::vector<SymbolInfo>> ExternalSymbols;
  bool DatabaseLoaded = false;

  /// Loads the symbol database from JSON file.
  void loadSymbolDatabase();

  /// Generates a location key for deduplication.
  static std::string getLocationKey(const SymbolInfo &Info);
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_DUPLICATEEXTERNALIDENTIFIERCHECK_H

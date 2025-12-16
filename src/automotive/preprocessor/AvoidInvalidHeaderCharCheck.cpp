//===--- AvoidInvalidHeaderCharCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidInvalidHeaderCharCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class InvalidHeaderCharPPCallbacks : public PPCallbacks {
public:
  explicit InvalidHeaderCharPPCallbacks(ClangTidyCheck &Check) : Check(Check) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {
    std::string InvalidChars = buildInvalidCharsList(FileName);
    if (InvalidChars.empty())
      return;

    Check.diag(FilenameRange.getBegin(),
               "header file name contains invalid character(s): %0")
        << InvalidChars;
  }

private:
  /// Build a comma-separated list of invalid characters found in the filename.
  static std::string buildInvalidCharsList(StringRef FileName) {
    llvm::SmallVector<const char *, 3> Found;
    if (FileName.contains('\''))
      Found.push_back("'");
    if (FileName.contains('"'))
      Found.push_back("\"");
    if (FileName.contains('\\'))
      Found.push_back("\\");

    std::string Result;
    for (size_t I = 0; I < Found.size(); ++I) {
      if (I > 0)
        Result += ", ";
      Result += Found[I];
    }
    return Result;
  }

  ClangTidyCheck &Check;
};

} // anonymous namespace

void AvoidInvalidHeaderCharCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<InvalidHeaderCharPPCallbacks>(*this));
}

} // namespace clang::tidy::automotive

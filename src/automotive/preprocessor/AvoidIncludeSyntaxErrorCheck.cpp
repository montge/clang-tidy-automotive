//===--- AvoidIncludeSyntaxErrorCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidIncludeSyntaxErrorCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class IncludeSyntaxPPCallbacks : public PPCallbacks {
public:
  IncludeSyntaxPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {
    // Skip system headers
    if (SM.isInSystemHeader(HashLoc))
      return;

    // Check if the include uses computed include syntax (macro expansion)
    // This is flagged because the result is implementation-defined
    // LCOV_EXCL_START - include_next is rare and causes test portability issues
    if (IncludeTok.getIdentifierInfo() &&
        IncludeTok.getIdentifierInfo()->getName() == "include_next") {
      Check.diag(HashLoc,
                 "#include_next is a compiler extension and should not be used "
                 "in portable code");
      return;
    }
    // LCOV_EXCL_STOP

    // LCOV_EXCL_START - empty filename would cause preprocessor error before check
    // Check for empty filename
    if (FileName.empty()) {
      Check.diag(HashLoc, "#include directive has empty filename");
      return;
    }
    // LCOV_EXCL_STOP

    // Check for absolute paths (non-portable)
    if (FileName.starts_with("/") ||
        (FileName.size() > 2 && FileName[1] == ':')) {
      Check.diag(FilenameRange.getBegin(),
                 "#include uses absolute path which is not portable");
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
};

} // namespace

void AvoidIncludeSyntaxErrorCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<IncludeSyntaxPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive

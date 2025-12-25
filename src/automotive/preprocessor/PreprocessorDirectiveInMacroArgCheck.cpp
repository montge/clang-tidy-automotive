//===--- PreprocessorDirectiveInMacroArgCheck.cpp - clang-tidy ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PreprocessorDirectiveInMacroArgCheck.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Token.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Check if a token sequence looks like a preprocessing directive
bool looksLikePreprocessorDirective(const Token *Tokens, unsigned NumTokens) {
  if (NumTokens == 0)
    return false;

  // Look for # followed by an identifier that could be a directive
  for (unsigned I = 0; I < NumTokens; ++I) {
    const Token &Tok = Tokens[I];
    if (Tok.is(tok::eof))
      break;

    // Check for hash token
    if (Tok.is(tok::hash)) {
      // Check if next token is a directive-like identifier
      if (I + 1 < NumTokens) {
        const Token &Next = Tokens[I + 1];
        if (Next.is(tok::raw_identifier) || Next.is(tok::identifier)) {
          StringRef Name = Next.getRawIdentifier();
          // Common preprocessing directives
          if (Name == "if" || Name == "ifdef" || Name == "ifndef" ||
              Name == "elif" || Name == "else" || Name == "endif" ||
              Name == "define" || Name == "undef" || Name == "include" ||
              Name == "line" || Name == "error" || Name == "pragma" ||
              Name == "warning") {
            return true;
          }
        }
      }
      // Standalone # in macro argument is also suspicious
      return true;
    }
  }

  return false;
}

class DirectiveInMacroArgPPCallbacks : public PPCallbacks {
public:
  DirectiveInMacroArgPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    if (!Args)
      return;

    SourceLocation MacroLoc = MacroNameTok.getLocation();
    if (MacroLoc.isInvalid() || SM.isInSystemHeader(MacroLoc))
      return;

    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();

    // Check each argument
    unsigned NumArgs = Args->getNumMacroArguments();
    for (unsigned I = 0; I < NumArgs; ++I) {
      const Token *ArgToks = Args->getUnexpArgument(I);
      if (!ArgToks)
        continue;

      unsigned NumArgTokens = Args->getArgLength(ArgToks);
      if (looksLikePreprocessorDirective(ArgToks, NumArgTokens)) {
        Check.diag(MacroLoc, "tokens resembling a preprocessing directive "
                             "shall not appear within macro argument of '%0'")
            << MacroName;
        return; // Report only once per macro expansion
      }
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
};

} // anonymous namespace

void PreprocessorDirectiveInMacroArgCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      std::make_unique<DirectiveInMacroArgPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive

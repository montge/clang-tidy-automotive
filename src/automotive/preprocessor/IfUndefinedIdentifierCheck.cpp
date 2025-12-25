//===--- IfUndefinedIdentifierCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IfUndefinedIdentifierCheck.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Token.h"
#include <vector>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class IfUndefinedIdentifierPPCallbacks : public PPCallbacks {
public:
  IfUndefinedIdentifierPPCallbacks(ClangTidyCheck &Check,
                                   const SourceManager &SM, Preprocessor *PP)
      : Check(Check), SM(SM), PP(PP) {}

  void If(SourceLocation Loc, SourceRange ConditionRange,
          ConditionValueKind ConditionValue) override {
    checkCondition(ConditionRange);
  }

  void Elif(SourceLocation Loc, SourceRange ConditionRange,
            ConditionValueKind ConditionValue, SourceLocation IfLoc) override {
    checkCondition(ConditionRange);
  }

  void Elifdef(SourceLocation Loc, const Token &MacroNameTok,
               const MacroDefinition &MD) override {
    // Elifdef is implicitly checking if defined, so no warning needed
  }

  void Elifndef(SourceLocation Loc, const Token &MacroNameTok,
                const MacroDefinition &MD) override {
    // Elifndef is implicitly checking if not defined, so no warning needed
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
  Preprocessor *PP;

  /// Check if a token represents a language keyword
  bool isKeyword(const Token &Tok) const {
    if (!Tok.is(tok::identifier))
      return false;

    const IdentifierInfo *II = Tok.getIdentifierInfo();
    if (!II)
      return false;

    // Check if this is a C/C++ keyword
    return II->isKeyword(PP->getLangOpts());
  }

  /// Parse tokens and check for undefined identifiers
  void checkCondition(SourceRange ConditionRange) {
    if (!ConditionRange.isValid())
      return;

    SourceLocation Start = ConditionRange.getBegin();
    SourceLocation End = ConditionRange.getEnd();

    if (SM.isInSystemHeader(Start))
      return;

    // Expand macro locations to spelling locations
    if (Start.isMacroID())
      Start = SM.getExpansionLoc(Start);
    if (End.isMacroID())
      End = SM.getExpansionLoc(End);

    // Get the source text
    bool Invalid = false;
    const char *StartPtr = SM.getCharacterData(Start, &Invalid);
    if (Invalid)
      return;

    const char *EndPtr = SM.getCharacterData(End, &Invalid);
    if (Invalid)
      return;

    // Calculate length including the last character
    size_t Length = EndPtr - StartPtr + 1;

    // Create a lexer for the condition range
    std::vector<Token> Tokens;
    Lexer Lex(Start, PP->getLangOpts(), StartPtr, StartPtr, StartPtr + Length);

    Token Tok;
    while (!Lex.LexFromRawLexer(Tok)) {
      if (Tok.is(tok::eof))
        break;
      Tokens.push_back(Tok);
    }

    // Analyze tokens for undefined identifiers
    for (size_t I = 0; I < Tokens.size(); ++I) {
      const Token &CurrentTok = Tokens[I];

      // Skip non-identifiers
      if (!CurrentTok.is(tok::identifier))
        continue;

      const IdentifierInfo *II = CurrentTok.getIdentifierInfo();
      if (!II)
        continue;

      // Skip C/C++ keywords
      if (isKeyword(CurrentTok))
        continue;

      // Check if this is the 'defined' operator
      StringRef Name = II->getName();
      if (Name == "defined") {
        // Skip the next token(s) which are the operand of 'defined'
        // Format: defined(X) or defined X
        size_t NextIdx = I + 1;
        if (NextIdx < Tokens.size()) {
          if (Tokens[NextIdx].is(tok::l_paren)) {
            // Skip the opening paren, the identifier, and closing paren
            // Skip until we find the matching closing paren
            int ParenDepth = 1;
            NextIdx++;
            while (NextIdx < Tokens.size() && ParenDepth > 0) {
              if (Tokens[NextIdx].is(tok::l_paren))
                ParenDepth++;
              else if (Tokens[NextIdx].is(tok::r_paren))
                ParenDepth--;
              NextIdx++;
            }
            I = NextIdx - 1; // -1 because the for loop will increment
          } else if (Tokens[NextIdx].is(tok::identifier)) {
            // Skip the identifier that follows 'defined'
            I = NextIdx;
          }
        }
        continue;
      }

      // Check if this identifier is defined as a macro
      MacroDefinition MD = PP->getMacroDefinition(II);
      if (!MD) {
        // Undefined identifier - issue warning
        Check.diag(CurrentTok.getLocation(),
                   "identifier '%0' used in preprocessing directive is not "
                   "defined; did you mean to use 'defined(%0)'?")
            << Name;
      }
    }
  }
};

} // anonymous namespace

void IfUndefinedIdentifierCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      std::make_unique<IfUndefinedIdentifierPPCallbacks>(*this, SM, PP));
}

} // namespace clang::tidy::automotive

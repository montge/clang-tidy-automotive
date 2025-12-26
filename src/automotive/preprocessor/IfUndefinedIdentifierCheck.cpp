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
#include <cctype>

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

  /// Check if a character can start an identifier
  static bool isIdentifierStart(char C) {
    return std::isalpha(static_cast<unsigned char>(C)) || C == '_';
  }

  /// Check if a character can continue an identifier
  static bool isIdentifierContinue(char C) {
    return std::isalnum(static_cast<unsigned char>(C)) || C == '_';
  }

  /// Extract identifiers from a string and check them
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
    if (Invalid || !StartPtr)
      return;

    const char *EndPtr = SM.getCharacterData(End, &Invalid);
    if (Invalid || !EndPtr)
      return;

    // Sanity check: End should be after Start
    if (EndPtr < StartPtr)
      return;

    // Get the actual end including the last token
    unsigned LastTokenLength =
        Lexer::MeasureTokenLength(End, SM, PP->getLangOpts());
    size_t Length = (EndPtr - StartPtr) + LastTokenLength;

    if (Length == 0 || Length > 10000)
      return;

    StringRef ConditionText(StartPtr, Length);

    // Parse identifiers manually from the condition text
    size_t I = 0;
    while (I < ConditionText.size()) {
      // Skip whitespace and non-identifier characters
      while (I < ConditionText.size() && !isIdentifierStart(ConditionText[I]) &&
             !std::isdigit(static_cast<unsigned char>(ConditionText[I]))) {
        ++I;
      }

      if (I >= ConditionText.size())
        break;

      // Skip numeric literals (including hex literals like 0xFF)
      if (std::isdigit(static_cast<unsigned char>(ConditionText[I]))) {
        // Skip the entire number including hex/octal/binary prefixes and suffixes
        while (I < ConditionText.size() &&
               (std::isalnum(static_cast<unsigned char>(ConditionText[I])) ||
                ConditionText[I] == '.' || ConditionText[I] == '\'')) {
          ++I;
        }
        continue;
      }

      // Found start of identifier
      size_t IdentStart = I;
      while (I < ConditionText.size() && isIdentifierContinue(ConditionText[I])) {
        ++I;
      }

      StringRef Ident = ConditionText.slice(IdentStart, I);
      if (Ident.empty())
        continue;

      // Skip the 'defined' operator and its operand
      if (Ident == "defined") {
        // Skip whitespace
        while (I < ConditionText.size() &&
               std::isspace(static_cast<unsigned char>(ConditionText[I]))) {
          ++I;
        }

        if (I < ConditionText.size() && ConditionText[I] == '(') {
          // defined(MACRO) form - skip to closing paren
          int Depth = 1;
          ++I;
          while (I < ConditionText.size() && Depth > 0) {
            if (ConditionText[I] == '(')
              ++Depth;
            else if (ConditionText[I] == ')')
              --Depth;
            ++I;
          }
        } else if (I < ConditionText.size() && isIdentifierStart(ConditionText[I])) {
          // defined MACRO form - skip the identifier
          while (I < ConditionText.size() && isIdentifierContinue(ConditionText[I])) {
            ++I;
          }
        }
        continue;
      }

      // Look up the identifier in the preprocessor's identifier table
      IdentifierInfo &II = PP->getIdentifierTable().get(Ident);

      // Skip C/C++ keywords
      if (II.isKeyword(PP->getLangOpts()))
        continue;

      // Check if this identifier is defined as a macro
      MacroDefinition MD = PP->getMacroDefinition(&II);
      if (!MD) {
        // Calculate the source location for this identifier
        SourceLocation IdentLoc = Start.getLocWithOffset(IdentStart);

        // Undefined identifier - issue warning
        Check.diag(IdentLoc,
                   "identifier '%0' used in preprocessing directive is not "
                   "defined; did you mean to use 'defined(%0)'?")
            << Ident;
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

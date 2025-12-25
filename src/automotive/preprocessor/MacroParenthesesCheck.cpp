//===--- MacroParenthesesCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MacroParenthesesCheck.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Token.h"
#include <set>
#include <string>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Check if a token is an operator that can cause precedence issues
bool isOperator(const Token &Tok) {
  return Tok.isOneOf(tok::plus, tok::minus, tok::star, tok::slash, tok::percent,
                     tok::amp, tok::pipe, tok::caret, tok::lessless,
                     tok::greatergreater, tok::ampamp, tok::pipepipe,
                     tok::equalequal, tok::exclaimequal, tok::less,
                     tok::lessequal, tok::greater, tok::greaterequal,
                     tok::question, tok::comma);
}

/// Check if a macro parameter usage is properly parenthesized
bool isProperlyParenthesized(const Token *Tokens, unsigned NumTokens,
                             unsigned ParamPos) {
  if (ParamPos == 0 || ParamPos >= NumTokens - 1)
    return false;

  const Token &PrevTok = Tokens[ParamPos - 1];
  const Token &NextTok = Tokens[ParamPos + 1];

  // Check if parameter is surrounded by parentheses
  return PrevTok.is(tok::l_paren) && NextTok.is(tok::r_paren);
}

/// Find unparenthesized parameter uses in a macro body
void checkMacroBody(const MacroInfo *MI, const std::set<std::string> &Params,
                    ClangTidyCheck &Check, const Token &MacroNameTok) {
  if (!MI || Params.empty())
    return;

  const auto &Tokens = MI->tokens();
  unsigned NumTokens = Tokens.size();

  for (unsigned I = 0; I < NumTokens; ++I) {
    const Token &Tok = Tokens[I];

    // Skip if not an identifier
    if (!Tok.is(tok::identifier))
      continue;

    // Check if this token is a macro parameter
    const IdentifierInfo *II = Tok.getIdentifierInfo();
    if (!II)
      continue;

    std::string ParamName = II->getName().str();
    if (Params.find(ParamName) == Params.end())
      continue;

    // Parameter found - check if it's in an expression context
    bool InExpression = false;
    bool NeedsParentheses = false;

    // Check if preceded by an operator
    if (I > 0 && isOperator(Tokens[I - 1])) {
      InExpression = true;
    }

    // Check if followed by an operator
    if (I + 1 < NumTokens && isOperator(Tokens[I + 1])) {
      InExpression = true;
    }

    // Check if parameter itself could be an expression (not stringified or
    // concatenated)
    bool IsStringified = (I > 0 && Tokens[I - 1].is(tok::hash));
    bool IsConcatenated =
        (I > 0 && Tokens[I - 1].is(tok::hashhash)) ||
        (I + 1 < NumTokens && Tokens[I + 1].is(tok::hashhash));

    // If stringified or concatenated, parentheses are not needed
    if (IsStringified || IsConcatenated)
      continue;

    // If in an expression context, check parenthesization
    if (InExpression) {
      if (!isProperlyParenthesized(Tokens.data(), NumTokens, I)) {
        NeedsParentheses = true;
      }
    }

    if (NeedsParentheses) {
      Check.diag(Tok.getLocation(),
                 "macro parameter '%0' shall be enclosed in parentheses")
          << ParamName;
    }
  }
}

class MacroParenthesesPPCallbacks : public PPCallbacks {
public:
  MacroParenthesesPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    SourceLocation MacroLoc = MacroNameTok.getLocation();
    if (MacroLoc.isInvalid() || SM.isInSystemHeader(MacroLoc))
      return;

    const MacroInfo *MI = MD->getMacroInfo();
    if (!MI || !MI->isFunctionLike())
      return;

    // Collect macro parameters
    std::set<std::string> Params;
    for (const auto &Param : MI->params()) {
      if (Param)
        Params.insert(Param->getName().str());
    }

    // Check macro body for unparenthesized parameter uses
    checkMacroBody(MI, Params, Check, MacroNameTok);
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
};

} // anonymous namespace

void MacroParenthesesCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<MacroParenthesesPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive

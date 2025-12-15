//===--- AvoidMultipleHashOperatorsCheck.cpp - clang-tidy -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidMultipleHashOperatorsCheck.h"
#include "clang/Basic/TokenKinds.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::tok;

namespace clang::tidy::automotive {

namespace {

class AvoidMultipleHashOperatorsPPCallbacks : public PPCallbacks {
public:
  explicit AvoidMultipleHashOperatorsPPCallbacks(ClangTidyCheck &Check)
      : Check(Check) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override;

private:
  ClangTidyCheck &Check;
};

void AvoidMultipleHashOperatorsPPCallbacks::MacroDefined(
    const Token &MacroNameTok, const MacroDirective *MD) {

  const MacroInfo *Info = MD->getMacroInfo();

  if (Info) {
    const Token *PrevTok = nullptr;

    for (const Token &Tok : Info->tokens()) {

      if (Tok.isOneOf(tok::hash, tok::hashhash)) {
        if (PrevTok) {
          SourceLocation TokLoc = Tok.getLocation();
          SourceLocation PrevTokLoc = PrevTok->getLocation();
          auto TokSpelling = getPunctuatorSpelling(Tok.getKind());
          auto PrevTokSpelling = getPunctuatorSpelling(PrevTok->getKind());

          Check.diag(TokLoc, "avoid '%0' operator after '%1' operator")
              << TokSpelling << PrevTokSpelling;
          Check.diag(PrevTokLoc, "location of '%0' operator",
                     DiagnosticIDs::Note)
              << PrevTokSpelling;
        }
        PrevTok = &Tok;
      }
    }
  }
}

} // anonymous namespace

void AvoidMultipleHashOperatorsCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      std::make_unique<AvoidMultipleHashOperatorsPPCallbacks>(*this));
}

} // namespace clang::tidy::automotive

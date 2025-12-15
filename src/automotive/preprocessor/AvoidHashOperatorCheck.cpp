//===--- AvoidHashOperatorCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidHashOperatorCheck.h"
#include "clang/Basic/TokenKinds.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::tok;

namespace clang::tidy::automotive {

namespace {

class AvoidHashOperatorPPCallbacks : public PPCallbacks {
public:
  explicit AvoidHashOperatorPPCallbacks(ClangTidyCheck &Check) : Check(Check) {}

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override;

private:
  ClangTidyCheck &Check;
};

void AvoidHashOperatorPPCallbacks::MacroDefined(const Token &MacroNameTok,
                                                const MacroDirective *MD) {
  const MacroInfo *Info = MD->getMacroInfo();

  if (Info) {
    for (const Token &Tok : Info->tokens()) {

      if (Tok.isOneOf(tok::hash, tok::hashhash)) {
        SourceLocation Loc = Tok.getLocation();
        Check.diag(Loc, "avoid preprocessor operator '%0'")
            << getPunctuatorSpelling(Tok.getKind());
      }
    }
  }
}

} // anonymous namespace

void AvoidHashOperatorCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<AvoidHashOperatorPPCallbacks>(*this));
}

} // namespace clang::tidy::automotive

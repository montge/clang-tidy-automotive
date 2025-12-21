//===--- AvoidSmallIntConstMacroCheck.cpp - clang-tidy --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidSmallIntConstMacroCheck.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class SmallIntConstMacroPPCallbacks : public PPCallbacks {
public:
  SmallIntConstMacroPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();

    // Check for small integer constant macros
    if (MacroName == "INT8_C" || MacroName == "INT16_C" ||
        MacroName == "UINT8_C" || MacroName == "UINT16_C") {

      SourceLocation Loc = MacroNameTok.getLocation();
      if (Loc.isInvalid() || SM.isInSystemHeader(Loc))
        return;

      Check.diag(Loc, "use of small integer constant macro '%0' may produce "
                      "implementation-defined behavior; consider using a "
                      "larger type or explicit constant")
          << MacroName;
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
};

} // anonymous namespace

void AvoidSmallIntConstMacroCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP,
    Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      std::make_unique<SmallIntConstMacroPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive

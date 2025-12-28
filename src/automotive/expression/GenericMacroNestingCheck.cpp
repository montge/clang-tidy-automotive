//===--- GenericMacroNestingCheck.cpp - clang-tidy -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "GenericMacroNestingCheck.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/Token.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {
/// PPCallbacks implementation to track macro expansions.
class GenericMacroNestingPPCallbacks : public PPCallbacks {
public:
  GenericMacroNestingPPCallbacks(GenericMacroNestingCheck &Check,
                                 const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    // Track macro expansions for later analysis
    // This is a simplified implementation - full implementation would
    // track the call chain and parameter flow
  }

private:
  GenericMacroNestingCheck &Check;
  const SourceManager &SM;
};
} // namespace

void GenericMacroNestingCheck::registerPPCallbacks(const SourceManager &SM,
                                                    Preprocessor *PP,
                                                    Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      std::make_unique<GenericMacroNestingPPCallbacks>(*this, SM));
}

void GenericMacroNestingCheck::registerMatchers(MatchFinder *Finder) {
  // Match generic selection expressions
  Finder->addMatcher(
      genericSelectionExpr(isExpansionInMainFile()).bind("genericSelection"),
      this);
}

void GenericMacroNestingCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *GenericSelection =
      Result.Nodes.getNodeAs<GenericSelectionExpr>("genericSelection");

  if (!GenericSelection)
    return;

  SourceLocation Loc = GenericSelection->getGenericLoc();

  // Check if this generic selection is from a macro
  if (!Loc.isMacroID())
    return;

  // Get the immediate macro expansion location
  SourceLocation ExpansionLoc =
      Result.SourceManager->getImmediateExpansionRange(Loc).getBegin();

  // Check if the expansion location is also from a macro (nested macro)
  if (ExpansionLoc.isMacroID()) {
    // This is a nested macro expansion - potential violation
    // Note: Full implementation would need to track the actual parameter flow
    // through preprocessor callbacks to determine if a generic type parameter
    // is being passed between macros

    // For now, emit a warning when we detect nested macro expansion
    // containing generic selections
    diag(Loc, "generic selection appears to be in a nested macro expansion; "
              "function-like macros should not invoke other macros with "
              "generic type parameters");
  }
}

} // namespace clang::tidy::automotive

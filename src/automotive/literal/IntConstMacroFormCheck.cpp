//===--- IntConstMacroFormCheck.cpp - clang-tidy --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IntConstMacroFormCheck.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Token.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

/// Information about an integer constant macro
struct IntConstMacroInfo {
  bool IsSigned;
  int BitWidth;
};

/// Get information about an integer constant macro by name
std::optional<IntConstMacroInfo> getIntConstMacroInfo(StringRef Name) {
  // Standard integer constant macros from <stdint.h>
  return llvm::StringSwitch<std::optional<IntConstMacroInfo>>(Name)
      .Case("INT8_C", IntConstMacroInfo{true, 8})
      .Case("INT16_C", IntConstMacroInfo{true, 16})
      .Case("INT32_C", IntConstMacroInfo{true, 32})
      .Case("INT64_C", IntConstMacroInfo{true, 64})
      .Case("UINT8_C", IntConstMacroInfo{false, 8})
      .Case("UINT16_C", IntConstMacroInfo{false, 16})
      .Case("UINT32_C", IntConstMacroInfo{false, 32})
      .Case("UINT64_C", IntConstMacroInfo{false, 64})
      .Case("INTMAX_C", IntConstMacroInfo{true, 64})
      .Case("UINTMAX_C", IntConstMacroInfo{false, 64})
      .Default(std::nullopt);
}

/// Check if a suffix is appropriate for the macro type
bool hasConflictingSuffix(StringRef ArgText, bool IsSigned) {
  // Convert to uppercase for comparison
  std::string Upper = ArgText.upper();

  // Check for explicit unsigned suffix on signed macro
  if (IsSigned) {
    if (Upper.find('U') != std::string::npos) {
      return true;
    }
  }

  // Check for long/long long suffixes that may conflict
  // The macro already handles the type, so explicit suffixes are suspicious
  size_t LCount = std::count(Upper.begin(), Upper.end(), 'L');
  if (LCount > 0) {
    return true; // L or LL suffix present
  }

  return false;
}

/// Check if the argument looks like an expression rather than a simple constant
bool looksLikeExpression(StringRef ArgText) {
  // Simple heuristic: if it contains operators or parentheses, it's an
  // expression
  for (char C : ArgText) {
    if (C == '+' || C == '-' || C == '*' || C == '/' || C == '%' || C == '(' ||
        C == ')' || C == '<' || C == '>' || C == '&' || C == '|' || C == '^' ||
        C == '~' || C == '!' || C == '?') {
      // Allow leading minus for negative numbers
      if (C == '-' && ArgText.starts_with("-")) {
        continue;
      }
      return true;
    }
  }
  return false;
}

class IntConstMacroFormPPCallbacks : public PPCallbacks {
public:
  IntConstMacroFormPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM,
                               const LangOptions &LangOpts)
      : Check(Check), SM(SM), LangOpts(LangOpts) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    if (!Args || Args->getNumMacroArguments() == 0)
      return;

    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();

    auto MacroInfo = getIntConstMacroInfo(MacroName);
    if (!MacroInfo)
      return;

    SourceLocation Loc = MacroNameTok.getLocation();
    if (Loc.isInvalid() || SM.isInSystemHeader(Loc))
      return;

    // Get the first argument's tokens
    const Token *ArgToks = Args->getUnexpArgument(0);
    if (!ArgToks)
      return;

    // Count tokens in the argument
    unsigned NumArgTokens = Args->getArgLength(ArgToks);
    if (NumArgTokens == 0)
      return;

    // Build the argument text from tokens
    std::string ArgText;
    SourceLocation FirstTokLoc;
    for (unsigned I = 0; I < NumArgTokens; ++I) {
      const Token &Tok = ArgToks[I];
      if (Tok.is(tok::eof))
        break;

      if (I == 0)
        FirstTokLoc = Tok.getLocation();

      // Get the spelling of this token
      bool Invalid = false;
      std::string TokSpelling = Lexer::getSpelling(Tok, SM, LangOpts, &Invalid);
      if (!Invalid) {
        ArgText += TokSpelling;
      }
    }

    if (ArgText.empty())
      return;

    // Trim whitespace
    ArgText.erase(0, ArgText.find_first_not_of(" \t\n\r"));
    ArgText.erase(ArgText.find_last_not_of(" \t\n\r") + 1);

    // Check 1: Argument should not be an expression
    if (looksLikeExpression(ArgText)) {
      Check.diag(Loc, "argument to integer constant macro '%0' should be a "
                      "simple integer constant, not an expression")
          << MacroName;
      return;
    }

    // Check 2: Argument should not have conflicting suffix
    if (hasConflictingSuffix(ArgText, MacroInfo->IsSigned)) {
      Check.diag(Loc, "argument to %select{signed|unsigned}0 integer constant "
                      "macro '%1' has conflicting type suffix")
          << (MacroInfo->IsSigned ? 0 : 1) << MacroName;
      return;
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
  const LangOptions &LangOpts;
};

} // anonymous namespace

void IntConstMacroFormCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<IntConstMacroFormPPCallbacks>(
      *this, SM, PP->getLangOpts()));
}

} // namespace clang::tidy::automotive

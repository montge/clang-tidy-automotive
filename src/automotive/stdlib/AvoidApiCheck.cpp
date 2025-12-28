//===--- AvoidApiCheck.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidApiCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::automotive {

using namespace clang::ast_matchers;

class AvoidApiPPCallbacks : public PPCallbacks {
public:
  AvoidApiPPCallbacks(AvoidApiCheck &Check, StringRef HeaderName,
                      ArrayRef<StringRef> MacroNames)
      : Check(Check), MacroNames(MacroNames), HeaderName(HeaderName) {}

  void InclusionDirective(SourceLocation DirectiveLoc,
                          const Token &IncludeToken, StringRef IncludedFilename,
                          bool IsAngled, CharSourceRange FilenameRange,
                          OptionalFileEntryRef IncludedFile,
                          StringRef SearchPath, StringRef RelativePath,
                          const Module *SuggestedModule, bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override;

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override;

private:
  AvoidApiCheck &Check;
  ArrayRef<StringRef> MacroNames;
  StringRef HeaderName;
};

void AvoidApiCheck::registerPPCallbacks(const SourceManager &SM,
                                        Preprocessor *PP,
                                        Preprocessor *ModuleExpanderPP) {
  // Only register callbacks if we have a header to check or functions to flag
  if (!HeaderName.empty() || !FunctionNames.empty()) {
    PP->addPPCallbacks(
        std::make_unique<AvoidApiPPCallbacks>(*this, HeaderName, FunctionNames));
  }
}

void AvoidApiCheck::registerMatchers(MatchFinder *Finder) {
  // Only register function matchers if we have functions to check
  if (!FunctionNames.empty()) {
    Finder->addMatcher(callExpr(callee(functionDecl(hasAnyName(FunctionNames))))
                           .bind("functionCall"),
                       this);
  }
}

void AvoidApiCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("functionCall");
  if (!Call)
    return;

  const FunctionDecl *FD = Call->getDirectCallee();
  if (!FD)
    return;

  diag(Call->getBeginLoc(),
       "use of '%0' is not allowed in safety-critical code")
      << FD->getName();
}

void AvoidApiPPCallbacks::InclusionDirective(
    SourceLocation DirectiveLoc, const Token &IncludeToken,
    StringRef IncludedFilename, bool IsAngled, CharSourceRange FilenameRange,
    OptionalFileEntryRef IncludedFile, StringRef SearchPath,
    StringRef RelativePath, const Module *SuggestedModule, bool ModuleImported,
    SrcMgr::CharacteristicKind FileType) {
  // Check if the included header matches the prohibited header
  // Compare the full filename or just the basename for flexibility
  if (!HeaderName.empty()) {
    bool matches = (IncludedFilename == HeaderName);
    // Also check if the filename ends with the header name (for path-qualified includes)
    if (!matches && IncludedFilename.ends_with(HeaderName)) {
      matches = true;
    }
    if (matches) {
      Check.diag(DirectiveLoc,
                 "inclusion of <%0> is not allowed in safety-critical code")
          << HeaderName;
    }
  }
}

void AvoidApiPPCallbacks::MacroExpands(const Token &MacroNameTok,
                                       const MacroDefinition &MD,
                                       SourceRange Range,
                                       const MacroArgs *Args) {
  // Check if the expanded macro is in the prohibited list
  StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
  for (StringRef ProhibitedMacro : MacroNames) {
    if (MacroName == ProhibitedMacro) {
      Check.diag(MacroNameTok.getLocation(),
                 "use of '%0' is not allowed in safety-critical code")
          << MacroName;
      return;
    }
  }
}

} // namespace clang::tidy::automotive

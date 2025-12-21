//===--- AvoidCtimeCppCheck.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCtimeCppCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class CtimeIncludeCallbacks : public PPCallbacks {
public:
  CtimeIncludeCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool IsAngled,
                          CharSourceRange FilenameRange,
                          OptionalFileEntryRef File, StringRef SearchPath,
                          StringRef RelativePath, const Module *SuggestedModule,
                          bool ModuleImported,
                          SrcMgr::CharacteristicKind FileType) override {
    if (SM.isInSystemHeader(HashLoc))
      return;

    if (FileName == "ctime" || FileName == "time.h") {
      Check.diag(HashLoc, "<ctime> header shall not be used; use <chrono>");
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
};

} // anonymous namespace

void AvoidCtimeCppCheck::registerPPCallbacks(const SourceManager &SM,
                                             Preprocessor *PP,
                                             Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<CtimeIncludeCallbacks>(*this, SM));
}

void AvoidCtimeCppCheck::registerMatchers(MatchFinder *Finder) {
  // Match calls to ctime functions
  Finder->addMatcher(
      callExpr(callee(functionDecl(hasAnyName(
                   "time", "clock", "difftime", "mktime", "asctime", "ctime",
                   "gmtime", "localtime", "strftime", "timespec_get"))))
          .bind("call"),
      this);
}

void AvoidCtimeCppCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *Call = Result.Nodes.getNodeAs<CallExpr>("call");
  if (!Call)
    return;

  if (Result.SourceManager->isInSystemHeader(Call->getBeginLoc()))
    return;

  const FunctionDecl *FD = Call->getDirectCallee();
  if (!FD)
    return;

  diag(Call->getBeginLoc(), "%0 from <ctime> shall not be used; use <chrono>")
      << FD->getName();
}

} // namespace clang::tidy::automotive

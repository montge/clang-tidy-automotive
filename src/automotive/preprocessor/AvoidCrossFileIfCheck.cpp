//===--- AvoidCrossFileIfCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidCrossFileIfCheck.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include <stack>

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class AvoidCrossFileIfPPCallbacks : public PPCallbacks {
public:
  AvoidCrossFileIfPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void If(SourceLocation Loc, SourceRange ConditionRange,
          ConditionValueKind ConditionValue) override {
    pushIfLocation(Loc);
  }

  void Ifdef(SourceLocation Loc, const Token &MacroNameTok,
             const MacroDefinition &MD) override {
    pushIfLocation(Loc);
  }

  void Ifndef(SourceLocation Loc, const Token &MacroNameTok,
              const MacroDefinition &MD) override {
    pushIfLocation(Loc);
  }

  void Elif(SourceLocation Loc, SourceRange ConditionRange,
            ConditionValueKind ConditionValue, SourceLocation IfLoc) override {
    checkSameFile(Loc, "elif");
  }

  void Elifdef(SourceLocation Loc, const Token &MacroNameTok,
               const MacroDefinition &MD) override {
    checkSameFile(Loc, "elifdef");
  }

  void Elifndef(SourceLocation Loc, const Token &MacroNameTok,
                const MacroDefinition &MD) override {
    checkSameFile(Loc, "elifndef");
  }

  void Else(SourceLocation Loc, SourceLocation IfLoc) override {
    checkSameFile(Loc, "else");
  }

  void Endif(SourceLocation Loc, SourceLocation IfLoc) override {
    checkSameFile(Loc, "endif");
    if (!IfStack.empty()) {
      IfStack.pop();
    }
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;

  struct IfInfo {
    SourceLocation Loc;
    FileID FileId;
  };
  std::stack<IfInfo> IfStack;

  void pushIfLocation(SourceLocation Loc) {
    if (Loc.isValid() && !SM.isInSystemHeader(Loc)) {
      IfInfo Info;
      Info.Loc = Loc;
      Info.FileId = SM.getFileID(Loc);
      IfStack.push(Info);
    }
  }

  void checkSameFile(SourceLocation Loc, const char *DirectiveName) {
    if (IfStack.empty() || !Loc.isValid())
      return;

    if (SM.isInSystemHeader(Loc))
      return;

    const IfInfo &Info = IfStack.top();
    FileID CurrentFileId = SM.getFileID(Loc);

    if (Info.FileId != CurrentFileId) {
      Check.diag(Loc, "#%0 directive shall be in the same file as the "
                      "corresponding #if/#ifdef/#ifndef")
          << DirectiveName;
      Check.diag(Info.Loc, "corresponding #if/#ifdef/#ifndef is here",
                 DiagnosticIDs::Note);
    }
  }
};

} // anonymous namespace

void AvoidCrossFileIfCheck::registerPPCallbacks(
    const SourceManager &SM, Preprocessor *PP, Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(
      ::std::make_unique<AvoidCrossFileIfPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive

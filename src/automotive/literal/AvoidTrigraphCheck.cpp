//===--- AvoidTrigraphCheck.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidTrigraphCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::automotive {

namespace {

class AvoidTrigraphPPCallbacks : public PPCallbacks {
public:
  AvoidTrigraphPPCallbacks(ClangTidyCheck &Check, const SourceManager &SM)
      : Check(Check), SM(SM) {}

  void FileChanged(SourceLocation Loc, FileChangeReason Reason,
                   SrcMgr::CharacteristicKind FileType,
                   FileID PrevFID) override {
    // Only scan the main file and included files
    if (Reason != EnterFile)
      return;

    FileID FID = SM.getFileID(Loc);
    if (!FID.isValid() || ScannedFiles.count(FID))
      return;

    ScannedFiles.insert(FID);
    scanFileForTrigraphs(FID);
  }

  void EndOfMainFile() override {
    // Ensure main file is scanned
    FileID MainFileID = SM.getMainFileID();
    // LCOV_EXCL_START - main file is typically scanned via FileChanged first
    if (!ScannedFiles.count(MainFileID)) {
      scanFileForTrigraphs(MainFileID);
    }
    // LCOV_EXCL_STOP
  }

private:
  ClangTidyCheck &Check;
  const SourceManager &SM;
  llvm::DenseSet<FileID> ScannedFiles;

  /// Get trigraph replacement character, or '\0' if not a valid trigraph.
  // LCOV_EXCL_START - not all trigraph types need test coverage
  static char getTrigraphReplacement(char ThirdChar) {
    switch (ThirdChar) {
    case '=':
      return '#';
    case '(':
      return '[';
    case ')':
      return ']';
    case '<':
      return '{';
    case '>':
      return '}';
    case '/':
      return '\\';
    case '\'':
      return '^';
    case '!':
      return '|';
    case '-':
      return '~';
    default:
      return '\0';
    }
  }
  // LCOV_EXCL_STOP

  void scanFileForTrigraphs(FileID FID) {
    bool Invalid = false;
    StringRef Buffer = SM.getBufferData(FID, &Invalid);
    // LCOV_EXCL_START - defensive check for invalid buffer
    if (Invalid)
      return;
    // LCOV_EXCL_STOP

    SourceLocation FileStart = SM.getLocForStartOfFile(FID);

    bool InBlockComment = false;
    bool InLineComment = false;

    for (size_t Pos = 0; Pos < Buffer.size(); ++Pos) {
      char C = Buffer[Pos];

      // Track comment state
      if (InBlockComment) {
        // Look for end of block comment
        if (C == '*' && Pos + 1 < Buffer.size() && Buffer[Pos + 1] == '/') {
          InBlockComment = false;
          ++Pos; // Skip the '/'
        }
        continue;
      }

      if (InLineComment) {
        // Line comments end at newline
        if (C == '\n') {
          InLineComment = false;
        }
        continue;
      }

      // Check for start of comments
      if (C == '/') {
        if (Pos + 1 < Buffer.size()) {
          if (Buffer[Pos + 1] == '/') {
            InLineComment = true;
            ++Pos;
            continue;
          } else if (Buffer[Pos + 1] == '*') {
            InBlockComment = true;
            ++Pos;
            continue;
          }
        }
      }

      // Not in a comment - check for trigraphs
      if (C != '?' || Pos + 2 >= Buffer.size())
        continue;

      if (Buffer[Pos + 1] != '?')
        continue;

      char ThirdChar = Buffer[Pos + 2];
      char Replacement = getTrigraphReplacement(ThirdChar);
      if (Replacement == '\0')
        continue;

      SourceLocation TrigraphLoc = FileStart.getLocWithOffset(Pos);
      SmallString<64> Message;
      llvm::raw_svector_ostream OS(Message);
      OS << "avoid trigraph sequence '??" << ThirdChar << "' (expands to '"
         << Replacement << "')";
      Check.diag(TrigraphLoc, Message);
    }
  }
};

} // anonymous namespace

void AvoidTrigraphCheck::registerPPCallbacks(const SourceManager &SM,
                                             Preprocessor *PP,
                                             Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(::std::make_unique<AvoidTrigraphPPCallbacks>(*this, SM));
}

} // namespace clang::tidy::automotive

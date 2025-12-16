//===--- TokenRange.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TOKENRANGE_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TOKENRANGE_H

#include "clang/Basic/AttrKinds.h"
#include "clang/Basic/CharInfo.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Lex/Lexer.h"
#include <optional>

namespace clang::tidy::automotive {

class TokenRange {
public:
  class TokenContext;
  class TokenIterator;

  TokenRange(SourceLocation StartLoc, SourceLocation EndLoc,
             const SourceManager &SM, const LangOptions &LangOpts)
      : Context(SM, LangOpts),
        StartIterator(TokenIterator(StartLoc.getLocWithOffset(-1), &Context)),
        EndIterator(TokenIterator(EndLoc)) {}

  class TokenContext {
  public:
    TokenContext(const SourceManager &SM, const LangOptions &LangOpts)
        : SM(SM), LangOpts(LangOpts) {}

    std::optional<Token> findNextToken(SourceLocation CurrentLoc) {
      return Lexer::findNextToken(CurrentLoc, SM, LangOpts);
    }

    bool isBeforeInTranslationUnit(SourceLocation StartLoc,
                                   SourceLocation EndLoc) {
      return SM.isBeforeInTranslationUnit(StartLoc, EndLoc);
    }

  private:
    const SourceManager &SM;
    const LangOptions &LangOpts;
  };

  class TokenIterator {
  public:
    using iterator_category = std::input_iterator_tag;

    TokenIterator(SourceLocation StartLoc, TokenContext *Context)
        : CurrentLoc(StartLoc), Context(Context) {
      advance();
    }

    explicit TokenIterator(SourceLocation EndLoc) : CurrentLoc(EndLoc) {}

    std::optional<Token> operator*() const { return Tok; }

    TokenIterator &operator++() {
      advance();
      return *this;
    }

    bool operator!=(const TokenIterator &Other) const {
      return Context->isBeforeInTranslationUnit(CurrentLoc, Other.CurrentLoc);
    }

  private:
    SourceLocation CurrentLoc;
    TokenContext *Context = nullptr;
    std::optional<Token> Tok;

    void advance() {
      Tok = Context->findNextToken(CurrentLoc);
      if (Tok) {
        CurrentLoc = Tok->getEndLoc();
      }
    }
  };

  TokenIterator begin() const { return StartIterator; }
  TokenIterator end() const { return EndIterator; }

private:
  TokenContext Context;
  TokenIterator StartIterator;
  TokenIterator EndIterator;
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_TOKENRANGE_H

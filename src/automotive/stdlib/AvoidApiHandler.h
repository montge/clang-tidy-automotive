//===--- AvoidApiHandler.h - clang-tidy -------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAPIHANDLER_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAPIHANDLER_H

#include "clang/Basic/SourceLocation.h"
#include "clang/Lex/PPCallbacks.h"
#include "llvm/ADT/StringRef.h"

namespace clang::tidy::automotive {

class MatchHeaderInfo {
public:
  const StringRef Name;
  const clang::SourceLocation Location;
  const Token &Tok;
  const bool IsAngled;
};

class MatchMacroInfo {
public:
  const StringRef Name;
  const MatchHeaderInfo *Header;
  const Token &Tok;
};

class AvoidApiHandler {
public:
  virtual ~AvoidApiHandler() = default;

  /// Override to check header inclusions. Default: no-op.
  virtual void checkHeader(const MatchHeaderInfo &Result) {
    // Default implementation: subclasses override to handle header checks
  }

  /// Override to check macro usage. Default: no-op.
  virtual void checkMacro(const MatchMacroInfo &Result) {
    // Default implementation: subclasses override to handle macro checks
  }

  /// Override to check function calls. Default: no-op.
  virtual void checkFunction(const MatchMacroInfo &Result) {
    // Default implementation: subclasses override to handle function checks
  }
};

} // namespace clang::tidy::automotive

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_AUTOMOTIVE_AVOIDAPIHANDLER_H

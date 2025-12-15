//===--- AvoidUndefCheck.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "AvoidUndefCheck.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang::ast_matchers;

namespace clang::tidy::automotive {

namespace {

class AvoidUndefPPCallbacks : public PPCallbacks {
public:
  explicit AvoidUndefPPCallbacks(ClangTidyCheck &Check) : Check(Check) {}

  void MacroUndefined(const Token &MacroNameTok, const MacroDefinition &MD,
                      const MacroDirective *Undef) override;

private:
  ClangTidyCheck &Check;
};

void AvoidUndefPPCallbacks::MacroUndefined(const Token &MacroNameTok,
                                           const MacroDefinition &MD,
                                           const MacroDirective *Undef) {
  if (Undef) {
    Check.diag(Undef->getLocation(), "avoid #undef");
  }
}

} // anonymous namespace

void AvoidUndefCheck::registerPPCallbacks(const SourceManager &SM,
                                          Preprocessor *PP,
                                          Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(::std::make_unique<AvoidUndefPPCallbacks>(*this));
}

} // namespace clang::tidy::automotive

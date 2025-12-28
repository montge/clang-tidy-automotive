//===--- PreprocessorFlowCheck.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PreprocessorFlowCheck.h"
#include "clang/Lex/Preprocessor.h"

namespace clang::tidy::automotive {

void PreprocessorFlowCheck::registerPPCallbacks(const SourceManager &SM,
                                                Preprocessor *PP,
                                                Preprocessor *ModuleExpanderPP) {
  // Infrastructure check - no specific callbacks registered
  // This class is intended to be extended by other preprocessor checks
}

} // namespace clang::tidy::automotive

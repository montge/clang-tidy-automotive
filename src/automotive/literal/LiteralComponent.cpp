//===--- LiteralComponent.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "LiteralComponent.h"
#include "AvoidLowercaseLiteralSuffixCheck.h"
#include "AvoidOctalNumberCheck.h"
#include "AvoidSmallIntConstMacroCheck.h"
#include "AvoidStringLiteralToNonConstCheck.h"
#include "AvoidTrigraphCheck.h"
#include "AvoidUnsignedLiteralWithoutSuffixCheck.h"
#include "IntConstMacroFormCheck.h"
#include "UnterminatedEscapeSequenceCheck.h"

namespace clang::tidy::automotive {

void LiteralComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidOctalNumberCheck>(
      "automotive-avoid-octal-number");
  CheckFactories.registerCheck<AvoidLowercaseLiteralSuffixCheck>(
      "automotive-avoid-lowercase-literal-suffix");
  CheckFactories.registerCheck<AvoidTrigraphCheck>("automotive-c23-adv-4.2");
  CheckFactories.registerCheck<UnterminatedEscapeSequenceCheck>(
      "automotive-unterminated-escape-sequence");
  CheckFactories.registerCheck<AvoidUnsignedLiteralWithoutSuffixCheck>(
      "automotive-c23-req-7.2");
  CheckFactories.registerCheck<AvoidStringLiteralToNonConstCheck>(
      "automotive-c23-req-7.4");

  // Rule 7.5 - Integer constant macro argument form (Mandatory)
  CheckFactories.registerCheck<IntConstMacroFormCheck>(
      "automotive-c25-mand-7.5");

  // Rule 7.6 - Small integer constant macros (Required)
  CheckFactories.registerCheck<AvoidSmallIntConstMacroCheck>(
      "automotive-c23-req-7.6");

  // MISRA C:2025 Rule 7.1 - Octal constants shall not be used (Required)
  CheckFactories.registerCheck<AvoidOctalNumberCheck>(
      "automotive-c25-req-7.1");

  // MISRA C:2025 Rule 7.2 - Unsigned suffix for unsigned constants (Required)
  CheckFactories.registerCheck<AvoidUnsignedLiteralWithoutSuffixCheck>(
      "automotive-c25-req-7.2");

  // MISRA C:2025 Rule 7.3 - Lowercase literal suffix (Required)
  CheckFactories.registerCheck<AvoidLowercaseLiteralSuffixCheck>(
      "automotive-c25-req-7.3");

  // MISRA C:2025 Rule 7.4 - String literal to non-const pointer (Required)
  CheckFactories.registerCheck<AvoidStringLiteralToNonConstCheck>(
      "automotive-c25-req-7.4");

  // MISRA C:2025 Rule 7.6 - Small integer constant macros (Required)
  CheckFactories.registerCheck<AvoidSmallIntConstMacroCheck>(
      "automotive-c25-req-7.6");

  // MISRA C:2025 Rule 4.2 - Trigraphs shall not be used (Advisory)
  CheckFactories.registerCheck<AvoidTrigraphCheck>("automotive-c25-adv-4.2");
}

} // namespace clang::tidy::automotive

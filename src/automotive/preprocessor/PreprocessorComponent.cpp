//===--- PreprocessorComponent.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PreprocessorComponent.h"
#include "AvoidCodeBeforeIncludeCheck.h"
#include "AvoidCrossFileIfCheck.h"
#include "AvoidHashOperatorCheck.h"
#include "AvoidIncludeSyntaxErrorCheck.h"
#include "AvoidInvalidHeaderCharCheck.h"
#include "AvoidLanguageExtensionCheck.h"
#include "AvoidMacroIdentifierConflictCheck.h"
#include "AvoidMacroNamedAsCkeywordCheck.h"
#include "AvoidMultipleHashOperatorsCheck.h"
#include "AvoidReservedMacroIdentifierCheck.h"
#include "AvoidUndefCheck.h"
#include "MacroParenthesesCheck.h"
#include "MissingHeaderGuardCheck.h"
#include "PreprocessorDirectiveInMacroArgCheck.h"
#include "UnusedMacroCheck.h"

namespace clang::tidy::automotive {

void PreprocessorComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidMacroNamedAsCkeywordCheck>(
      "automotive-avoid-macro-named-as-ckeyword");

  CheckFactories.registerCheck<UnusedMacroCheck>("automotive-unused-macro");
  CheckFactories.registerCheck<AvoidUndefCheck>("automotive-avoid-undef");
  CheckFactories.registerCheck<AvoidHashOperatorCheck>(
      "automotive-avoid-hash-operator");
  CheckFactories.registerCheck<AvoidMultipleHashOperatorsCheck>(
      "automotive-avoid-multiple-hash-operators");

  // Rule 20.1 - Include directive order
  CheckFactories.registerCheck<AvoidCodeBeforeIncludeCheck>(
      "automotive-avoid-code-before-include");

  // Rule 20.2 - Invalid characters in header names
  CheckFactories.registerCheck<AvoidInvalidHeaderCharCheck>(
      "automotive-avoid-invalid-header-char");

  // Rule 20.3 - Include syntax validation
  CheckFactories.registerCheck<AvoidIncludeSyntaxErrorCheck>(
      "automotive-c23-req-20.3");

  // Rule 21.1 - Reserved macro identifiers
  CheckFactories.registerCheck<AvoidReservedMacroIdentifierCheck>(
      "automotive-avoid-reserved-macro-identifier");

  // Rule 20.15 - #define/#undef reserved identifiers (Required)
  CheckFactories.registerCheck<AvoidReservedMacroIdentifierCheck>(
      "automotive-c23-req-20.15");

  // Rule 5.4 - Macro identifier conflicts
  CheckFactories.registerCheck<AvoidMacroIdentifierConflictCheck>(
      "automotive-c23-req-5.4");

  // Directive 4.10 - Header guard protection
  CheckFactories.registerCheck<MissingHeaderGuardCheck>(
      "automotive-c23-req-dir-4.10");

  // Rule 1.2 - Language extensions (Advisory)
  CheckFactories.registerCheck<AvoidLanguageExtensionCheck>(
      "automotive-c23-adv-1.2");

  // Rule 20.14 - #if/#endif same file (Required)
  CheckFactories.registerCheck<AvoidCrossFileIfCheck>(
      "automotive-c23-req-20.14");

  // Rule 20.6 - Preprocessing directive in macro argument (Required)
  CheckFactories.registerCheck<PreprocessorDirectiveInMacroArgCheck>(
      "automotive-c25-req-20.6");

  // Rule 20.7 - Macro parameter parenthesization (Required)
  CheckFactories.registerCheck<MacroParenthesesCheck>(
      "automotive-c25-req-20.7");
}

} // namespace clang::tidy::automotive

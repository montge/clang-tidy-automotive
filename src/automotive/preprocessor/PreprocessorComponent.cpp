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
#include "IfExpressionValueCheck.h"
#include "IfUndefinedIdentifierCheck.h"
#include "MacroParenthesesCheck.h"
#include "MissingHeaderGuardCheck.h"
#include "PreprocessorDirectiveInMacroArgCheck.h"
#include "ReservedIdentifierMacroCheck.h"
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
  CheckFactories.registerCheck<ReservedIdentifierMacroCheck>(
      "automotive-c25-req-20.15");

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

  // Rule 20.8 - #if/#elif expression shall evaluate to 0 or 1 (Required)
  CheckFactories.registerCheck<IfExpressionValueCheck>(
      "automotive-c25-req-20.8");

  // Rule 20.9 - Undefined identifiers in #if/#elif (Required)
  CheckFactories.registerCheck<IfUndefinedIdentifierCheck>(
      "automotive-c25-req-20.9");

  // MISRA C++:2023 Rule 19.2.2 - Macro parameters shall be parenthesized
  // (Required)
  CheckFactories.registerCheck<MacroParenthesesCheck>(
      "automotive-cpp23-req-19.2.2");

  // MISRA C++:2023 Rule 19.3.3 - #if expression shall evaluate to 0 or 1
  // (Required)
  CheckFactories.registerCheck<IfExpressionValueCheck>(
      "automotive-cpp23-req-19.3.3");

  // MISRA C++:2023 Rule 19.3.4 - Undefined identifiers in #if/#elif
  // shall be avoided (Required)
  CheckFactories.registerCheck<IfUndefinedIdentifierCheck>(
      "automotive-cpp23-req-19.3.4");

  // MISRA C:2025 Rule 20.1 - #include preceded only by preprocessor directives
  CheckFactories.registerCheck<AvoidCodeBeforeIncludeCheck>(
      "automotive-c25-req-20.1");

  // MISRA C:2025 Rule 20.2 - Invalid characters in header names (Required)
  CheckFactories.registerCheck<AvoidInvalidHeaderCharCheck>(
      "automotive-c25-req-20.2");

  // MISRA C:2025 Rule 20.3 - #include syntax (Required)
  CheckFactories.registerCheck<AvoidIncludeSyntaxErrorCheck>(
      "automotive-c25-req-20.3");

  // MISRA C:2025 Rule 20.4 - Macro shall not be named after keyword (Required)
  CheckFactories.registerCheck<AvoidMacroNamedAsCkeywordCheck>(
      "automotive-c25-req-20.4");

  // MISRA C:2025 Rule 20.5 - #undef should not be used (Advisory)
  CheckFactories.registerCheck<AvoidUndefCheck>("automotive-c25-adv-20.5");

  // MISRA C:2025 Rule 20.10 - # and ## operators should not be used (Advisory)
  CheckFactories.registerCheck<AvoidHashOperatorCheck>(
      "automotive-c25-adv-20.10");

  // MISRA C:2025 Rule 20.11 - # or ## shall not occur more than once (Required)
  CheckFactories.registerCheck<AvoidMultipleHashOperatorsCheck>(
      "automotive-c25-req-20.11");

  // MISRA C:2025 Rule 20.14 - #if/#endif in same file (Required)
  CheckFactories.registerCheck<AvoidCrossFileIfCheck>(
      "automotive-c25-req-20.14");

  // MISRA C:2025 Rule 5.4 - Macro identifier conflicts (Required)
  CheckFactories.registerCheck<AvoidMacroIdentifierConflictCheck>(
      "automotive-c25-req-5.4");

  // MISRA C:2025 Directive 4.10 - Header guard (Required)
  CheckFactories.registerCheck<MissingHeaderGuardCheck>(
      "automotive-c25-req-dir-4.10");

  // MISRA C:2025 Rule 1.2 - Language extensions (Advisory)
  CheckFactories.registerCheck<AvoidLanguageExtensionCheck>(
      "automotive-c25-adv-1.2");
}

} // namespace clang::tidy::automotive

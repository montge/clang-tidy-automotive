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
  CheckFactories.registerCheck<AvoidSmallIntConstMacroCheck>(
      "automotive-c23-req-7.6");
  CheckFactories.registerCheck<AvoidStringLiteralToNonConstCheck>(
      "automotive-c23-req-7.4");
  CheckFactories.registerCheck<AvoidTrigraphCheck>("automotive-c23-adv-4.2");
  CheckFactories.registerCheck<AvoidTrigraphCheck>("automotive-c25-adv-4.2");
  CheckFactories.registerCheck<AvoidUnsignedLiteralWithoutSuffixCheck>(
      "automotive-c23-req-7.2");
  CheckFactories.registerCheck<IntConstMacroFormCheck>(
      "automotive-c25-mand-7.5");
  CheckFactories.registerCheck<UnterminatedEscapeSequenceCheck>(
      "automotive-unterminated-escape-sequence");
}

} // namespace clang::tidy::automotive

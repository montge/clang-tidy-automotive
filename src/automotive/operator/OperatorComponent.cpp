//===--- OperatorComponent.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "OperatorComponent.h"
#include "AvoidCommaOperatorCheck.h"
#include "WrongShiftOperandCheck.h"

namespace clang::tidy::automotive {

void OperatorComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidCommaOperatorCheck>(
      "automotive-avoid-comma-operator");

  // Rule 12.2 - Shift operator bounds (Required)
  CheckFactories.registerCheck<WrongShiftOperandCheck>(
      "automotive-c23-req-12.2");

  // MISRA C:2025 Rule 12.2 - Shift operator bounds (Required)
  CheckFactories.registerCheck<WrongShiftOperandCheck>(
      "automotive-c25-req-12.2");

  // MISRA C:2025 Rule 12.1 - Comma operator shall not be used (Required)
  CheckFactories.registerCheck<AvoidCommaOperatorCheck>(
      "automotive-c25-req-12.1");
}

} // namespace clang::tidy::automotive

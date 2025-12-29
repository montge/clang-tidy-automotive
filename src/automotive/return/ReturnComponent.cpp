//===--- ReturnComponent.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ReturnComponent.h"
#include "MissingReturnValueHandlingCheck.h"
#include "MultipleReturnStmtCheck.h"
#include "NoReturnVoidCheck.h"

namespace clang::tidy::automotive {

void ReturnComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<MultipleReturnStmtCheck>(
      "automotive-avoid-multiple-return-stmt");
  CheckFactories.registerCheck<MissingReturnValueHandlingCheck>(
      "automotive-missing-return-value-handling");
  CheckFactories.registerCheck<NoReturnVoidCheck>(
      "automotive-missing-return-void");

  // MISRA C:2012 Rule 15.5 - A function should have a single point of exit
  // (Advisory)
  CheckFactories.registerCheck<MultipleReturnStmtCheck>(
      "automotive-c23-adv-15.5");

  // MISRA C:2025 Rule 17.12 - A function shall have a single point of exit
  // (Required)
  CheckFactories.registerCheck<MultipleReturnStmtCheck>(
      "automotive-c25-req-17.12");
}

} // namespace clang::tidy::automotive

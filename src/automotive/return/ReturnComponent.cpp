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

  // MISRA C:2025 Rule 17.4 - Single return point (Advisory)
  CheckFactories.registerCheck<MultipleReturnStmtCheck>(
      "automotive-c25-adv-17.4");

  // MISRA C:2025 Rule 17.7 - Return value shall be used (Required)
  CheckFactories.registerCheck<MissingReturnValueHandlingCheck>(
      "automotive-c25-req-17.7");

  // MISRA C:2025 Rule 17.8 - Non-void function shall return value (Required)
  CheckFactories.registerCheck<NoReturnVoidCheck>("automotive-c25-req-17.8");
}

} // namespace clang::tidy::automotive

//===--- ArrayComponent.cpp - clang-tidy ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ArrayComponent.h"
#include "AvoidFlexibleArrayMemberCheck.h"
#include "AvoidPartialArrayInitCheck.h"
#include "AvoidPointerArithmeticBoundsCheck.h"
#include "AvoidPointerArithmeticCheck.h"
#include "AvoidVariableLengthArrayCheck.h"
#include "MissingExternalArraySizeCheck.h"

namespace clang::tidy::automotive {

void ArrayComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidPartialArrayInitCheck>(
      "automotive-avoid-partial-array-init");
  CheckFactories.registerCheck<AvoidFlexibleArrayMemberCheck>(
      "automotive-avoid-flexible-array-member");
  CheckFactories.registerCheck<AvoidVariableLengthArrayCheck>(
      "automotive-avoid-variable-length-array");
  CheckFactories.registerCheck<MissingExternalArraySizeCheck>(
      "automotive-missing-external-array-size");

  // Rules 18.1, 18.2, 18.3, 18.5 - Pointer arithmetic bounds (Required)
  CheckFactories.registerCheck<AvoidPointerArithmeticBoundsCheck>(
      "automotive-c23-req-18.1");

  // Rule 18.4 - Pointer arithmetic (Advisory)
  CheckFactories.registerCheck<AvoidPointerArithmeticCheck>(
      "automotive-c23-adv-18.4");
}

} // namespace clang::tidy::automotive

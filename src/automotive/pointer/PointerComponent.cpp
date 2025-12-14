//===--- PointerComponent.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PointerComponent.h"
#include "AvoidAtomicVoidPointerCheck.h"
#include "AvoidAutoAddressEscapeCheck.h"
#include "AvoidCastRemovingQualifierCheck.h"
#include "AvoidIncompatiblePointerCastCheck.h"
#include "AvoidIncompatiblePointerConversionCheck.h"
#include "AvoidPointerIntegerCastCheck.h"
#include "WrongNullPointerValueCheck.h"

namespace clang::tidy::automotive {

void PointerComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  // Rules 11.2, 11.5, 11.6, 11.7 - Pointer conversions (Required)
  CheckFactories.registerCheck<AvoidIncompatiblePointerConversionCheck>(
      "automotive-c23-req-11.2");

  // Rule 11.3 - Incompatible pointer cast (Required)
  CheckFactories.registerCheck<AvoidIncompatiblePointerCastCheck>(
      "automotive-c23-req-11.3");

  // Rule 11.4 - Pointer to/from integer cast (Advisory)
  CheckFactories.registerCheck<AvoidPointerIntegerCastCheck>(
      "automotive-c23-adv-11.4");

  // Rule 11.8 - Cast removing const/volatile (Required)
  CheckFactories.registerCheck<AvoidCastRemovingQualifierCheck>(
      "automotive-c23-req-11.8");

  CheckFactories.registerCheck<WrongNullPointerValueCheck>(
      "automotive-wrong-null-pointer-value");

  // Rule 18.6 - Address of automatic object escaping (Required)
  CheckFactories.registerCheck<AvoidAutoAddressEscapeCheck>(
      "automotive-c23-req-18.6");
}

} // namespace clang::tidy::automotive

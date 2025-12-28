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
#include "AvoidConstQualifierRemovalCheck.h"
#include "AvoidImplicitNullComparisonCheck.h"
#include "AvoidIncompatiblePointerCastCheck.h"
#include "AvoidIncompatiblePointerConversionCheck.h"
#include "AvoidPointerArithmeticC25Check.h"
#include "AvoidPointerIntegerCastCheck.h"
#include "AvoidPointerToFloatCastCheck.h"
#include "AvoidPointerTypeCastCheck.h"
#include "WrongNullPointerValueCheck.h"
#include "WrongPointerArithmeticCheck.h"

namespace clang::tidy::automotive {

void PointerComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  // Rule 11.2 - Incompatible pointer conversion (Required)
  // Note: This check also handles rules 11.5, 11.6, 11.7
  CheckFactories.registerCheck<AvoidIncompatiblePointerConversionCheck>(
      "automotive-c23-req-11.2");

  // Rule 11.5 - void pointer to object pointer conversion (Advisory)
  CheckFactories.registerCheck<AvoidIncompatiblePointerConversionCheck>(
      "automotive-c23-adv-11.5");

  // Rule 11.6 - void pointer to/from arithmetic conversion (Required)
  CheckFactories.registerCheck<AvoidIncompatiblePointerConversionCheck>(
      "automotive-c23-req-11.6");

  // Rule 11.3 - Incompatible pointer cast (Required)
  CheckFactories.registerCheck<AvoidIncompatiblePointerCastCheck>(
      "automotive-c23-req-11.3");
  CheckFactories.registerCheck<AvoidPointerTypeCastCheck>(
      "automotive-c25-req-11.3");

  // Rule 11.4 - Pointer to/from integer cast (Advisory)
  CheckFactories.registerCheck<AvoidPointerIntegerCastCheck>(
      "automotive-c23-adv-11.4");

  // Rule 11.7 - Pointer to float cast (MISRA C:2025, Required)
  CheckFactories.registerCheck<AvoidPointerToFloatCastCheck>(
      "automotive-c25-req-11.7");

  // Rule 11.8 - Cast removing const/volatile (Required)
  CheckFactories.registerCheck<AvoidCastRemovingQualifierCheck>(
      "automotive-c23-req-11.8");
  CheckFactories.registerCheck<AvoidConstQualifierRemovalCheck>(
      "automotive-c25-req-11.8");

  // Rule 11.9 - Atomic void pointer (MISRA C:2023, Required)
  CheckFactories.registerCheck<AvoidAtomicVoidPointerCheck>(
      "automotive-c23-req-11.9");

  // Rule 11.11 - Implicit null comparison (MISRA C:2023, Required)
  CheckFactories.registerCheck<AvoidImplicitNullComparisonCheck>(
      "automotive-c23-req-11.11");

  // Rule 18.4 - Pointer arithmetic (MISRA C:2025, Advisory)
  CheckFactories.registerCheck<AvoidPointerArithmeticC25Check>(
      "automotive-c25-adv-18.4");

  // Rule 18.6 - Auto address escape (MISRA C:2023, Required)
  CheckFactories.registerCheck<AvoidAutoAddressEscapeCheck>(
      "automotive-c23-req-18.6");

  CheckFactories.registerCheck<WrongNullPointerValueCheck>(
      "automotive-wrong-null-pointer-value");

  // MISRA C:2025 Rule 18.10 - Pointer arithmetic on array elements (Mandatory)
  CheckFactories.registerCheck<WrongPointerArithmeticCheck>(
      "automotive-c25-mand-18.10");
}

} // namespace clang::tidy::automotive

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

namespace clang::tidy::automotive {

void PointerComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  // Rules 11.2, 11.5, 11.6, 11.7 - Pointer conversions (Required)
  CheckFactories.registerCheck<AvoidIncompatiblePointerConversionCheck>(
      "automotive-c23-req-11.2");

  // Rule 11.3 - Incompatible pointer cast (Required)
  CheckFactories.registerCheck<AvoidIncompatiblePointerCastCheck>(
      "automotive-c23-req-11.3");

  // C:2025 Rule 11.3 - Incompatible pointer cast (Required)
  CheckFactories.registerCheck<AvoidPointerTypeCastCheck>(
      "automotive-c25-req-11.3");

  // Rule 11.4 - Pointer to/from integer cast (Advisory)
  CheckFactories.registerCheck<AvoidPointerIntegerCastCheck>(
      "automotive-c23-adv-11.4");

  // Rule 11.7 - Pointer to/from floating-point cast (Required)
  CheckFactories.registerCheck<AvoidPointerToFloatCastCheck>(
      "automotive-c25-req-11.7");

  // Rule 11.8 - Cast removing const/volatile (Required)
  CheckFactories.registerCheck<AvoidCastRemovingQualifierCheck>(
      "automotive-c23-req-11.8");

  // C:2025 Rule 11.8 - Cast removing const/volatile (Required)
  CheckFactories.registerCheck<AvoidConstQualifierRemovalCheck>(
      "automotive-c25-req-11.8");

  // Rule 11.9 - Atomic void pointer (Required)
  CheckFactories.registerCheck<AvoidAtomicVoidPointerCheck>(
      "automotive-c23-req-11.9");

  CheckFactories.registerCheck<WrongNullPointerValueCheck>(
      "automotive-wrong-null-pointer-value");

  // Rule 18.6 - Address of automatic object escaping (Required)
  CheckFactories.registerCheck<AvoidAutoAddressEscapeCheck>(
      "automotive-c23-req-18.6");

  // Rule 11.11 - Implicit NULL comparison (Required)
  CheckFactories.registerCheck<AvoidImplicitNullComparisonCheck>(
      "automotive-c23-req-11.11");

  // C:2025 Rule 18.4 - Pointer arithmetic (Advisory)
  CheckFactories.registerCheck<AvoidPointerArithmeticC25Check>(
      "automotive-c25-adv-18.4");

  // MISRA C:2025 Rule 11.2 - Pointer conversions (Required)
  CheckFactories.registerCheck<AvoidIncompatiblePointerConversionCheck>(
      "automotive-c25-req-11.2");

  // MISRA C:2025 Rule 11.4 - Pointer to/from integer cast (Advisory)
  CheckFactories.registerCheck<AvoidPointerIntegerCastCheck>(
      "automotive-c25-adv-11.4");

  // MISRA C:2025 Rule 11.9 - NULL macro for null pointer (Required)
  CheckFactories.registerCheck<WrongNullPointerValueCheck>(
      "automotive-c25-req-11.9");

  // MISRA C:2025 Rule 11.11 - Implicit NULL comparison (Required)
  CheckFactories.registerCheck<AvoidImplicitNullComparisonCheck>(
      "automotive-c25-req-11.11");

  // MISRA C:2025 Rule 18.6 - Address of automatic object escaping (Required)
  CheckFactories.registerCheck<AvoidAutoAddressEscapeCheck>(
      "automotive-c25-req-18.6");
}

} // namespace clang::tidy::automotive

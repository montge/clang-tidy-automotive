//===--- TypeComponent.cpp - clang-tidy -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TypeComponent.h"
#include "AvoidAtomicVoidCheck.h"
#include "AvoidMixedDesignatorCheck.h"
#include "AvoidPointerTypedefCheck.h"
#include "AvoidUnionCheck.h"
#include "DuplicateTagNameCheck.h"
#include "DuplicateTypedefNameCheck.h"
#include "ExplicitEnumeratorValuesCheck.h"
#include "ImplicitIntCheck.h"
#include "UniqueEnumValuesCheck.h"
#include "UnusedTypeCheck.h"

namespace clang::tidy::automotive {

void TypeComponent::addCheckFactories(ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidUnionCheck>("automotive-avoid-union");
  CheckFactories.registerCheck<DuplicateTagNameCheck>("automotive-c23-req-5.7");
  CheckFactories.registerCheck<DuplicateTypedefNameCheck>(
      "automotive-c23-req-5.6");
  CheckFactories.registerCheck<ImplicitIntCheck>("automotive-implicit-int");
  CheckFactories.registerCheck<UniqueEnumValuesCheck>(
      "automotive-unique-enum-value");
  CheckFactories.registerCheck<UnusedTypeCheck>("automotive-unused-type");

  // Rule 9.2 - Explicit enumerator values (Required)
  CheckFactories.registerCheck<ExplicitEnumeratorValuesCheck>(
      "automotive-c23-req-9.2");

  // Dir 4.8 - Pointer hiding in typedefs (Advisory)
  CheckFactories.registerCheck<AvoidPointerTypedefCheck>(
      "automotive-c23-adv-dir-4.8");

  // Rule 11.10 - Atomic void restriction (Required)
  CheckFactories.registerCheck<AvoidAtomicVoidCheck>(
      "automotive-c23-req-11.10");

  // MISRA C:2025 Rule 9.6 - Mixed designators in initializer
  CheckFactories.registerCheck<AvoidMixedDesignatorCheck>(
      "automotive-c25-req-9.6");
}

} // namespace clang::tidy::automotive

//===--- StorageComponent.cpp - clang-tidy --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StorageComponent.h"
#include "AvoidExternInSourceFileCheck.h"
#include "AvoidGlobalObjectCheck.h"
#include "AvoidMultipleAlignmentCheck.h"
#include "AvoidReservedIdentifierDeclarationCheck.h"
#include "AvoidRestrictTypeCheck.h"
#include "AvoidTentativeDefinitionInHeaderCheck.h"
#include "AvoidUninitializedReadCheck.h"
#include "AvoidZeroAlignmentCheck.h"
#include "DuplicateExternalIdentifierCheck.h"
#include "ExternalSymbolCollectorCheck.h"
#include "MissingPriorDeclarationCheck.h"
#include "MissingStaticForInternalCheck.h"
#include "MultipleExternalDefinitionCheck.h"
#include "StaticStorageClassCheck.h"
#include "ThreadObjectStorageCheck.h"

namespace clang::tidy::automotive {

void StorageComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  // Rule 21.2 - Reserved identifier declarations (Required)
  CheckFactories.registerCheck<AvoidReservedIdentifierDeclarationCheck>(
      "automotive-c23-req-21.2");

  // Rule 8.9 - Object at block scope (Advisory)
  CheckFactories.registerCheck<AvoidGlobalObjectCheck>(
      "automotive-c23-adv-8.9");

  // Rule 9.1 - Uninitialized read (Required)
  CheckFactories.registerCheck<AvoidUninitializedReadCheck>(
      "automotive-c23-req-9.1");

  // Rule 5.8 - Duplicate external identifier (Required)
  CheckFactories.registerCheck<DuplicateExternalIdentifierCheck>(
      "automotive-c23-req-5.8");

  // Rule 8.4 - Missing prior declaration (Required)
  CheckFactories.registerCheck<MissingPriorDeclarationCheck>(
      "automotive-c23-req-8.4");

  // Rule 8.6 - Multiple external definition (Required)
  CheckFactories.registerCheck<MultipleExternalDefinitionCheck>(
      "automotive-c23-req-8.6");

  // Rule 8.7 - Missing static for internal (Advisory)
  CheckFactories.registerCheck<MissingStaticForInternalCheck>(
      "automotive-c23-adv-8.7");

  // Rule 8.8 - Static storage class (Required)
  CheckFactories.registerCheck<StaticStorageClassCheck>(
      "automotive-c25-req-8.8");

  // Rule 8.16 - Zero alignment (Advisory)
  CheckFactories.registerCheck<AvoidZeroAlignmentCheck>(
      "automotive-c25-adv-8.16");

  // Rule 8.17 - Multiple alignment (Advisory)
  CheckFactories.registerCheck<AvoidMultipleAlignmentCheck>(
      "automotive-c25-adv-8.17");

  // Rule 8.19 - Extern in source file (Advisory)
  CheckFactories.registerCheck<AvoidExternInSourceFileCheck>(
      "automotive-c25-adv-8.19");

  // Rule 22.13 - Thread object storage (Required)
  CheckFactories.registerCheck<ThreadObjectStorageCheck>(
      "automotive-c25-req-22.13");

  // External symbol collector (utility)
  CheckFactories.registerCheck<ExternalSymbolCollectorCheck>(
      "automotive-collect-external-symbols");

  // Rule 8.18 - Tentative definition in header (Required)
  CheckFactories.registerCheck<AvoidTentativeDefinitionInHeaderCheck>(
      "automotive-c23-req-8.18");

  // Tentative definition in header (legacy name)
  CheckFactories.registerCheck<AvoidTentativeDefinitionInHeaderCheck>(
      "automotive-avoid-tentative-definition-in-header");

  CheckFactories.registerCheck<AvoidRestrictTypeCheck>(
      "automotive-avoid-restrict-type");
  CheckFactories.registerCheck<MissingStaticForInternalCheck>(
      "automotive-missing-static-internal-linkage");
}

} // namespace clang::tidy::automotive

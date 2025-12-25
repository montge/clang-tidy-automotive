//===--- StdlibComponent.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StdlibComponent.h"
#include "AtoXCheck.h"
#include "AvoidBsearchQsortCheck.h"
#include "AvoidCtypeHeaderCheck.h"
#include "AvoidMemcmpOnStringsCheck.h"
#include "AvoidSetjmpHeaderCheck.h"
#include "AvoidSignalHeaderCheck.h"
#include "AvoidStdargHeaderCheck.h"
#include "AvoidStdioHeaderCheck.h"
#include "AvoidStdlibMemoryCheck.h"
#include "AvoidStdlibRandCheck.h"
#include "AvoidTgmathHeaderCheck.h"
#include "AvoidTimeHeaderCheck.h"
#include "AvoidWcharHeaderCheck.h"
#include "AvoidstdlibsystemcallCheck.h"
#include "ErrnoTestingCheck.h"
#include "ErrnoUsageCheck.h"
#include "ExitCheck.h"
#include "MemorySyncOrderCheck.h"
#include "TgmathConsistentTypeCheck.h"
#include "TgmathOperandTypeCheck.h"

namespace clang::tidy::automotive {

void StdlibComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<AvoidStdargHeaderCheck>(
      "automotive-avoid-stdarg-header");
  CheckFactories.registerCheck<AvoidSetjmpHeaderCheck>(
      "automotive-avoid-setjmp-header");
  CheckFactories.registerCheck<AvoidSignalHeaderCheck>(
      "automotive-avoid-signal-header");
  CheckFactories.registerCheck<AtoXCheck>("automotive-avoid-ascii-to-number");
  CheckFactories.registerCheck<ExitCheck>("automotive-avoid-stdlib-exit");

  CheckFactories.registerCheck<AvoidstdlibsystemcallCheck>(
      "automotive-avoid-stdlib-system");
  CheckFactories.registerCheck<AvoidStdlibRandCheck>(
      "automotive-avoid-stdlib-rand");

  // Rule 21.24 - Random number generator functions (Required)
  // Extends 21.12 (rand) to include srand
  CheckFactories.registerCheck<AvoidStdlibRandCheck>(
      "automotive-c23-req-21.24");

  // Rule 21.3 / Dir 4.12 - Dynamic memory allocation (Required)
  CheckFactories.registerCheck<AvoidStdlibMemoryCheck>(
      "automotive-c23-req-21.3");

  // Rule 21.6 - Standard library I/O functions (Required)
  CheckFactories.registerCheck<AvoidStdioHeaderCheck>(
      "automotive-c23-req-21.6");

  // Rule 21.10 - Standard library time functions (Required)
  CheckFactories.registerCheck<AvoidTimeHeaderCheck>(
      "automotive-c23-req-21.10");

  // Rule 21.21 - Wide character functions (Required)
  CheckFactories.registerCheck<AvoidWcharHeaderCheck>(
      "automotive-c23-req-21.21");

  // Rule 21.11 - Type-generic math header (Required)
  CheckFactories.registerCheck<AvoidTgmathHeaderCheck>(
      "automotive-c23-req-21.11");

  // Rule 21.13 - ctype.h functions (Mandatory)
  CheckFactories.registerCheck<AvoidCtypeHeaderCheck>(
      "automotive-c23-mand-21.13");

  // Rule 21.9 - bsearch/qsort prohibition (Required)
  CheckFactories.registerCheck<AvoidBsearchQsortCheck>(
      "automotive-c23-req-21.9");

  // Rule 22.8 - errno shall be zero before call (Required)
  // Rule 22.9 - errno shall be tested after call (Required)
  // Combined check for errno-setting function usage
  CheckFactories.registerCheck<ErrnoUsageCheck>("automotive-c25-req-22.8");
  CheckFactories.registerCheck<ErrnoUsageCheck>("automotive-c25-req-22.9");

  // Rule 22.10 - errno testing (Required)
  // errno shall only be tested after an errno-setting function
  CheckFactories.registerCheck<ErrnoTestingCheck>("automotive-c23-req-22.10");

  // Rule 21.22 - tgmath.h operand types (Mandatory)
  // All operand arguments to type-generic macros shall have appropriate type
  CheckFactories.registerCheck<TgmathOperandTypeCheck>(
      "automotive-c25-mand-21.22");

  // Rule 21.23 - tgmath consistent types (Required)
  // All operand arguments to multi-argument tgmath macros shall have same type
  CheckFactories.registerCheck<TgmathConsistentTypeCheck>(
      "automotive-c25-req-21.23");

  // Rule 21.25 - Memory synchronization order (Required)
  // All memory synchronization operations shall be executed in sequentially
  // consistent order
  CheckFactories.registerCheck<MemorySyncOrderCheck>(
      "automotive-c25-req-21.25");

  // Rule 21.14 - memcmp shall not be used for string comparison (Required)
  CheckFactories.registerCheck<AvoidMemcmpOnStringsCheck>(
      "automotive-c25-req-21.14");
}

} // namespace clang::tidy::automotive

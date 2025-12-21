//===--- StdlibComponent.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StdlibComponent.h"
#include "AtoXCheck.h"
#include "AvoidCtypeHeaderCheck.h"
#include "AvoidSetjmpHeaderCheck.h"
#include "AvoidSignalHeaderCheck.h"
#include "AvoidStdargHeaderCheck.h"
#include "AvoidStdioHeaderCheck.h"
#include "AvoidStdlibMemoryCheck.h"
#include "AvoidStdlibRandCheck.h"
#include "AvoidstdlibsystemcallCheck.h"
#include "AvoidTgmathHeaderCheck.h"
#include "AvoidTimeHeaderCheck.h"
#include "AvoidWcharHeaderCheck.h"
#include "ExitCheck.h"

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
}

} // namespace clang::tidy::automotive

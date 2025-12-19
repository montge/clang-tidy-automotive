//===--- Cpp23Component.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Cpp23Component.h"
#include "AvoidCStyleCastCheck.h"
#include "AvoidDynamicCastCheck.h"
#include "AvoidNarrowingConversionCheck.h"
#include "AvoidSlicingCheck.h"
#include "AvoidThrowingDestructorCheck.h"
#include "ExplicitConstructorCheck.h"
#include "NoexceptMoveCheck.h"
#include "RuleOfFiveCheck.h"
#include "VirtualDestructorCheck.h"

namespace clang::tidy::automotive {

void Cpp23Component::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  // MISRA C++:2023 Rule 8.2.1 - C-style casts shall not be used
  CheckFactories.registerCheck<AvoidCStyleCastCheck>(
      "automotive-cpp23-req-8.2.1");

  // MISRA C++:2023 Rule 15.3.1 - Avoid object slicing
  CheckFactories.registerCheck<AvoidSlicingCheck>("automotive-cpp23-req-15.3");

  // MISRA C++:2023 Rule 15.7.1 - Base classes with virtual functions need
  // virtual destructors
  CheckFactories.registerCheck<VirtualDestructorCheck>(
      "automotive-cpp23-req-15.7");

  // MISRA C++:2023 Rule 15.1.3 - Single-argument constructors shall be explicit
  CheckFactories.registerCheck<ExplicitConstructorCheck>(
      "automotive-cpp23-req-15.1.3");

  // MISRA C++:2023 Rule 18.4.2 - Move operations shall be noexcept
  CheckFactories.registerCheck<NoexceptMoveCheck>(
      "automotive-cpp23-req-18.4.2");

  // MISRA C++:2023 Rule 8.4.1 - Narrowing conversions shall not be used
  CheckFactories.registerCheck<AvoidNarrowingConversionCheck>(
      "automotive-cpp23-req-8.4.1");

  // MISRA C++:2023 Rule 15.0.1 - Rule of Five for special member functions
  CheckFactories.registerCheck<RuleOfFiveCheck>("automotive-cpp23-req-15.0.1");

  // MISRA C++:2023 Rule 8.2.5 - dynamic_cast should not be used
  CheckFactories.registerCheck<AvoidDynamicCastCheck>(
      "automotive-cpp23-adv-8.2.5");

  // MISRA C++:2023 Rule 18.4.1 - Destructors shall not throw exceptions
  CheckFactories.registerCheck<AvoidThrowingDestructorCheck>(
      "automotive-cpp23-req-18.4.1");
}

} // namespace clang::tidy::automotive

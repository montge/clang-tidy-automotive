//===--- Cpp23Component.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Cpp23Component.h"
#include "AvoidCStyleCastCheck.h"
#include "AvoidConstCastCheck.h"
#include "AvoidCtimeCppCheck.h"
#include "AvoidDynamicCastCheck.h"
#include "AvoidDynamicMemoryCppCheck.h"
#include "AvoidExceptionCheck.h"
#include "AvoidFloatingPointEqualityCheck.h"
#include "AvoidImplicitConversionCheck.h"
#include "AvoidImplicitConversionOperatorCheck.h"
#include "AvoidNarrowingConversionCheck.h"
#include "AvoidNonCompoundBodyCheck.h"
#include "AvoidReinterpretCastCheck.h"
#include "AvoidSingleClauseSwitchCppCheck.h"
#include "AvoidSlicingCheck.h"
#include "AvoidThrowInNoexceptCheck.h"
#include "AvoidThrowingDestructorCheck.h"
#include "AvoidUnionCppCheck.h"
#include "ExplicitConstructorCheck.h"
#include "NoexceptMoveCheck.h"
#include "ProperConceptDefinitionCheck.h"
#include "RuleOfFiveCheck.h"
#include "UnconstrainedTemplateCheck.h"
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

  // MISRA C++:2023 Rule 8.2.3 - const_cast shall not be used
  CheckFactories.registerCheck<AvoidConstCastCheck>(
      "automotive-cpp23-req-8.2.3");

  // MISRA C++:2023 Rule 8.2.4 - reinterpret_cast shall not be used
  CheckFactories.registerCheck<AvoidReinterpretCastCheck>(
      "automotive-cpp23-req-8.2.4");

  // MISRA C++:2023 Rule 15.1.2 - Conversion operators shall be explicit
  CheckFactories.registerCheck<AvoidImplicitConversionOperatorCheck>(
      "automotive-cpp23-req-15.1.2");

  // MISRA C++:2023 Rule 18.4.3 - Throw in noexcept function calls
  // std::terminate
  CheckFactories.registerCheck<AvoidThrowInNoexceptCheck>(
      "automotive-cpp23-req-18.4.3");

  // MISRA C++:2023 Rule 8.3.1 - Implicit conversions shall be explicit
  CheckFactories.registerCheck<AvoidImplicitConversionCheck>(
      "automotive-cpp23-req-8.3.1");

  // MISRA C++:2023 Rule 17.0.1 - Template parameters shall be constrained
  CheckFactories.registerCheck<UnconstrainedTemplateCheck>(
      "automotive-cpp23-adv-17.0.1");

  // MISRA C++:2023 Rule 17.1.1 - Concepts shall be properly defined
  CheckFactories.registerCheck<ProperConceptDefinitionCheck>(
      "automotive-cpp23-adv-17.1.1");

  // MISRA C++:2023 Rule 9.3 - Compound statement body
  CheckFactories.registerCheck<AvoidNonCompoundBodyCheck>(
      "automotive-cpp23-req-9.3");

  // MISRA C++:2023 Rule 9.4 - Switch minimum clauses
  CheckFactories.registerCheck<AvoidSingleClauseSwitchCppCheck>(
      "automotive-cpp23-req-9.4");

  // MISRA C++:2023 Rule 12.3 - Union prohibition
  CheckFactories.registerCheck<AvoidUnionCppCheck>("automotive-cpp23-req-12.3");

  // MISRA C++:2023 Rule 18.1 - Exception prohibition
  CheckFactories.registerCheck<AvoidExceptionCheck>(
      "automotive-cpp23-req-18.1");

  // MISRA C++:2023 Rule 21.6 - Dynamic memory prohibition
  CheckFactories.registerCheck<AvoidDynamicMemoryCppCheck>(
      "automotive-cpp23-req-21.6");

  // MISRA C++:2023 Rule 21.10 - ctime prohibition
  CheckFactories.registerCheck<AvoidCtimeCppCheck>(
      "automotive-cpp23-req-21.10");

  // MISRA C++:2023 Rule 6.7 - Floating-point equality comparison
  CheckFactories.registerCheck<AvoidFloatingPointEqualityCheck>(
      "automotive-cpp23-req-6.7");
}

} // namespace clang::tidy::automotive

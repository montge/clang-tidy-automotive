//===--- ExpressionComponent.cpp - clang-tidy -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ExpressionComponent.h"
#include "AvoidAssignmentInExpressionCheck.h"
#include "AvoidAtomicMemberAccessCheck.h"
#include "AvoidCharacterArithmeticCheck.h"
#include "AvoidCompositeExpressionMismatchCheck.h"
#include "AvoidConstantWrapAroundCheck.h"
#include "AvoidEssentialTypeMismatchCheck.h"
#include "AvoidInappropriateCastCheck.h"
#include "AvoidInappropriateEssentialTypeCheck.h"
#include "AvoidNarrowingCompoundAssignmentCheck.h"
#include "AvoidNonBooleanInConditionCheck.h"
#include "AvoidSideEffectInInitializerCheck.h"
#include "AvoidSideEffectInLogicalOperandCheck.h"
#include "AvoidSideEffectInSizeofCheck.h"
#include "AvoidSizeofArrayParameterCheck.h"
#include "GenericDefaultPositionCheck.h"
#include "GenericEssentialTypeCheck.h"
#include "GenericFromMacroCheck.h"
#include "GenericImplicitPointerCheck.h"
#include "GenericNonDefaultCheck.h"
#include "GenericSideEffectCheck.h"
#include "InvariantControlCheck.h"

namespace clang::tidy::automotive {

void ExpressionComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  // Rule 10.1 - Inappropriate essential type (Required)
  CheckFactories.registerCheck<AvoidInappropriateEssentialTypeCheck>(
      "automotive-c23-req-10.1");

  // Rule 10.2 - Character arithmetic (Required)
  CheckFactories.registerCheck<AvoidCharacterArithmeticCheck>(
      "automotive-c23-req-10.2");

  // Rule 10.3 - Narrowing assignment (Required)
  CheckFactories.registerCheck<AvoidNarrowingCompoundAssignmentCheck>(
      "automotive-c23-req-10.3");

  // Rule 10.4 - Essential type mismatch (Required)
  CheckFactories.registerCheck<AvoidEssentialTypeMismatchCheck>(
      "automotive-c23-req-10.4");

  // Rule 10.5 - Inappropriate cast (Advisory)
  CheckFactories.registerCheck<AvoidInappropriateCastCheck>(
      "automotive-c23-adv-10.5");

  // Rules 10.6, 10.7, 10.8 - Composite expression mismatch (Required)
  CheckFactories.registerCheck<AvoidCompositeExpressionMismatchCheck>(
      "automotive-c23-req-10.6");

  // Rule 12.4 - Constant expression wrap-around (Advisory)
  CheckFactories.registerCheck<AvoidConstantWrapAroundCheck>(
      "automotive-c23-adv-12.4");

  // Rules 13.1, 13.2, 13.3 - Side effects in initializers (Required)
  CheckFactories.registerCheck<AvoidSideEffectInInitializerCheck>(
      "automotive-c23-req-13.1");

  // Rule 13.4 - Assignment used as expression (Advisory)
  CheckFactories.registerCheck<AvoidAssignmentInExpressionCheck>(
      "automotive-c23-adv-13.4");

  // Rule 13.5 - Side effects in logical operator operands (Required)
  CheckFactories.registerCheck<AvoidSideEffectInLogicalOperandCheck>(
      "automotive-c23-req-13.5");

  // Rule 13.6 - Side effects in sizeof operand (Mandatory)
  CheckFactories.registerCheck<AvoidSideEffectInSizeofCheck>(
      "automotive-c23-mand-13.6");

  // Rule 12.5 - sizeof on array parameter (Required)
  CheckFactories.registerCheck<AvoidSizeofArrayParameterCheck>(
      "automotive-c23-req-12.5");

  // Rule 14.3 - Invariant controlling expression (Required)
  CheckFactories.registerCheck<InvariantControlCheck>(
      "automotive-c23-req-14.3");

  // Rule 14.4 - Non-boolean in condition (Required)
  CheckFactories.registerCheck<AvoidNonBooleanInConditionCheck>(
      "automotive-c23-req-14.4");

  // Rule 12.6 - Atomic struct member access (Required)
  CheckFactories.registerCheck<AvoidAtomicMemberAccessCheck>(
      "automotive-c25-req-12.6");

  // Rule 23.1 - Generic from macro (Advisory)
  CheckFactories.registerCheck<GenericFromMacroCheck>(
      "automotive-c25-adv-23.1");

  // Rule 23.2 - Generic side effects in controlling expression (Required)
  CheckFactories.registerCheck<GenericSideEffectCheck>(
      "automotive-c25-req-23.2");

  // Rule 23.3 - Generic non-default association (Advisory)
  CheckFactories.registerCheck<GenericNonDefaultCheck>(
      "automotive-c25-adv-23.3");

  // Rule 23.5 - Generic implicit pointer conversion (Advisory)
  CheckFactories.registerCheck<GenericImplicitPointerCheck>(
      "automotive-c25-adv-23.5");

  // Rule 23.6 - Generic essential type match (Required)
  CheckFactories.registerCheck<GenericEssentialTypeCheck>(
      "automotive-c25-req-23.6");

  // Rule 23.8 - Generic default position (Required)
  CheckFactories.registerCheck<GenericDefaultPositionCheck>(
      "automotive-c25-req-23.8");
}

} // namespace clang::tidy::automotive

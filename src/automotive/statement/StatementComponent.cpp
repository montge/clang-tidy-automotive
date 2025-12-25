//===--- StatementComponent.cpp - clang-tidy ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "StatementComponent.h"
#include "AvoidBooleanInSwitchCheck.h"
#include "AvoidGotoCheck.h"
#include "AvoidSingleClauseSwitchCheck.h"
#include "ForwardGotoLabelCheck.h"
#include "GotoLabelSameBlockCheck.h"
#include "MissingBreakInCaseStmtCheck.h"
#include "MissingCompoundStmtCheck.h"
#include "MissingDefaultInSwitchStmtCheck.h"
#include "MissingElseCheck.h"
#include "MultipleLoopTerminatorsCheck.h"
#include "UnstructuredSwitchStmtCheck.h"
#include "UnusedLabelCheck.h"
#include "WellFormedForLoopCheck.h"
#include "WellFormedSwitchCheck.h"
#include "WrongOrderInSwitchStmtCheck.h"

namespace clang::tidy::automotive {

void StatementComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<UnusedLabelCheck>("automotive-unused-label");
  CheckFactories.registerCheck<AvoidGotoCheck>("automotive-avoid-goto");
  CheckFactories.registerCheck<ForwardGotoLabelCheck>(
      "automotive-forward-goto-label");
  CheckFactories.registerCheck<GotoLabelSameBlockCheck>(
      "automotive-c23-req-15.2");

  // MISRA C++:2023 Rule 9.6.2 - A goto statement shall reference a label
  // in an enclosing block (Required)
  CheckFactories.registerCheck<GotoLabelSameBlockCheck>(
      "automotive-cpp23-req-9.6.2");

  // MISRA C++:2023 Rule 9.6.3 - A goto statement shall jump forward
  // (Required)
  CheckFactories.registerCheck<ForwardGotoLabelCheck>(
      "automotive-cpp23-req-9.6.3");

  CheckFactories.registerCheck<MissingBreakInCaseStmtCheck>(
      "automotive-missing-break-in-case");
  CheckFactories.registerCheck<MissingDefaultInSwitchStmtCheck>(
      "automotive-missing-default-in-switch");
  CheckFactories.registerCheck<WrongOrderInSwitchStmtCheck>(
      "automotive-c23-req-16.5");
  CheckFactories.registerCheck<AvoidBooleanInSwitchCheck>(
      "automotive-avoid-boolean-in-switch");
  CheckFactories.registerCheck<UnstructuredSwitchStmtCheck>(
      "automotive-x-req-16.2");
  /* 16.2 Inspiration: m
   * caseStmt(unless(hasParent(compoundStmt(hasParent(switchStmt()))))) */

  /* 15.
  ---------------------------------------------------------------- */
  CheckFactories.registerCheck<MissingCompoundStmtCheck>(
      "automotive-missing-compound");
  CheckFactories.registerCheck<MissingElseCheck>("automotive-missing-else");

  // MISRA C:2025 Rule 15.7 - else if terminated with else (Required)
  CheckFactories.registerCheck<MissingElseCheck>("automotive-c25-req-15.7");

  // Rule 15.4 - Multiple loop terminators (Advisory)
  CheckFactories.registerCheck<MultipleLoopTerminatorsCheck>(
      "automotive-c23-adv-15.4");

  // Rule 16.6 - Switch minimum clauses (Required)
  CheckFactories.registerCheck<AvoidSingleClauseSwitchCheck>(
      "automotive-c23-req-16.6");

  // Rule 14.2 - Well-formed for loop (Required)
  CheckFactories.registerCheck<WellFormedForLoopCheck>(
      "automotive-c25-req-14.2");

  // Rule 16.1 - Well-formed switch (Required)
  CheckFactories.registerCheck<WellFormedSwitchCheck>(
      "automotive-c25-req-16.1");

  // MISRA C:2025 Rule 15.2 - goto shall reference label in enclosing block
  CheckFactories.registerCheck<GotoLabelSameBlockCheck>(
      "automotive-c25-req-15.2");

  // MISRA C:2025 Rule 15.3 - goto shall only jump forward (Required)
  CheckFactories.registerCheck<ForwardGotoLabelCheck>(
      "automotive-c25-req-15.3");

  // MISRA C:2025 Rule 15.1 - goto should not be used (Advisory)
  CheckFactories.registerCheck<AvoidGotoCheck>("automotive-c25-adv-15.1");

  // MISRA C:2025 Rule 16.4 - switch shall have default clause (Required)
  CheckFactories.registerCheck<MissingDefaultInSwitchStmtCheck>(
      "automotive-c25-req-16.4");

  // MISRA C:2025 Rule 16.5 - default shall be first or last (Required)
  CheckFactories.registerCheck<WrongOrderInSwitchStmtCheck>(
      "automotive-c25-req-16.5");

  // MISRA C:2025 Rule 16.3 - break shall terminate switch clause (Required)
  CheckFactories.registerCheck<MissingBreakInCaseStmtCheck>(
      "automotive-c25-req-16.3");

  // MISRA C:2025 Rule 16.6 - switch shall have at least two clauses (Required)
  CheckFactories.registerCheck<AvoidSingleClauseSwitchCheck>(
      "automotive-c25-req-16.6");

  // MISRA C:2025 Rule 15.6 - loop/selection body shall be compound (Required)
  CheckFactories.registerCheck<MissingCompoundStmtCheck>(
      "automotive-c25-req-15.6");

  // MISRA C:2025 Rule 15.4 - single break/goto to exit loop (Advisory)
  CheckFactories.registerCheck<MultipleLoopTerminatorsCheck>(
      "automotive-c25-adv-15.4");
}

} // namespace clang::tidy::automotive

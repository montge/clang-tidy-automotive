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
#include "DeadCodeCheck.h"
#include "ForwardGotoLabelCheck.h"
#include "GotoLabelSameBlockCheck.h"
#include "MissingBreakInCaseStmtCheck.h"
#include "MissingCompoundStmtCheck.h"
#include "MissingDefaultInSwitchStmtCheck.h"
#include "MissingElseCheck.h"
#include "MultipleLoopTerminatorsCheck.h"
#include "UnreachableCodeCheck.h"
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

  // Rule 15.4 - Multiple loop terminators (Advisory)
  CheckFactories.registerCheck<MultipleLoopTerminatorsCheck>(
      "automotive-c23-adv-15.4");

  // Rule 14.2 - Well-formed for loop
  CheckFactories.registerCheck<WellFormedForLoopCheck>(
      "automotive-c25-req-14.2");

  // Rule 16.1 - Well-formed switch statement
  CheckFactories.registerCheck<WellFormedSwitchCheck>(
      "automotive-c25-req-16.1");

  // Rule 16.6 - Avoid single clause switch
  CheckFactories.registerCheck<AvoidSingleClauseSwitchCheck>(
      "automotive-c23-req-16.6");

  // Rule 2.1 - Unreachable code (Required)
  CheckFactories.registerCheck<UnreachableCodeCheck>(
      "automotive-c23-req-2.1");

  // Rule 2.2 - Dead code (Required)
  CheckFactories.registerCheck<DeadCodeCheck>("automotive-c25-req-2.2");
}

} // namespace clang::tidy::automotive

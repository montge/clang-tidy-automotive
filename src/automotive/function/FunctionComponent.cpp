//===--- FunctionComponent.cpp - clang-tidy -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "FunctionComponent.h"
#include "AvoidFunctionParameterModificationCheck.h"
#include "AvoidNonConstPointerParamCheck.h"
#include "AvoidNoreturnNonVoidCheck.h"
#include "AvoidNoreturnReturnCheck.h"
#include "AvoidQualifiedFunctionTypeCheck.h"
#include "AvoidStaticInArrayParamCheck.h"
#include "FunctionDeclarationMismatchCheck.h"
#include "FunctionIdentifierUsageCheck.h"
#include "ImplicitFunctionDeclCheck.h"
#include "ObsolescentFeaturesCheck.h"
#include "StaticInlineCheck.h"
#include "UncompleteFunctionPrototypeCheck.h"

namespace clang::tidy::automotive {

void FunctionComponent::addCheckFactories(
    ClangTidyCheckFactories &CheckFactories) {

  CheckFactories.registerCheck<UncompleteFunctionPrototypeCheck>(
      "automotive-uncomplete-function-prototype");
  CheckFactories.registerCheck<FunctionDeclarationMismatchCheck>(
      "automotive-function-declaration-mismatch");
  CheckFactories.registerCheck<StaticInlineCheck>(
      "automotive-missing-static-inline");

  CheckFactories.registerCheck<ImplicitFunctionDeclCheck>(
      "automotive-implicit-function-decl");
  CheckFactories.registerCheck<AvoidFunctionParameterModificationCheck>(
      "automotive-avoid-function-parameter-modification");

  // Rule 17.6 - Static keyword in array parameters (Mandatory)
  CheckFactories.registerCheck<AvoidStaticInArrayParamCheck>(
      "automotive-c23-mand-17.6");

  // Rule 17.10 - Noreturn with non-void return type (Required)
  CheckFactories.registerCheck<AvoidNoreturnNonVoidCheck>(
      "automotive-c23-req-17.10");

  // Rule 17.12 - Function identifier usage (Advisory)
  CheckFactories.registerCheck<FunctionIdentifierUsageCheck>(
      "automotive-c25-adv-17.12");

  // Rule 17.13 - Qualified function type (Required)
  CheckFactories.registerCheck<AvoidQualifiedFunctionTypeCheck>(
      "automotive-c23-req-17.13");

  // Rule 1.5 - Obsolescent language features (Required)
  CheckFactories.registerCheck<ObsolescentFeaturesCheck>(
      "automotive-c25-req-1.5");

  // MISRA C:2025 Rule 17.6 - Static keyword in array parameters (Mandatory)
  CheckFactories.registerCheck<AvoidStaticInArrayParamCheck>(
      "automotive-c25-mand-17.6");

  // MISRA C:2025 Rule 17.10 - Noreturn with non-void return type (Required)
  CheckFactories.registerCheck<AvoidNoreturnNonVoidCheck>(
      "automotive-c25-req-17.10");

  // MISRA C:2025 Rule 17.13 - Qualified function type (Required)
  CheckFactories.registerCheck<AvoidQualifiedFunctionTypeCheck>(
      "automotive-c25-req-17.13");

  // MISRA C:2025 Rule 17.1 - stdarg.h shall not be used (Required)
  // Registered in StdlibComponent

  // MISRA C:2025 Rule 17.3 - Implicit function declaration (Mandatory)
  CheckFactories.registerCheck<ImplicitFunctionDeclCheck>(
      "automotive-c25-mand-17.3");

  // MISRA C:2025 Rule 17.7 - Return value not discarded (Required)
  // Note: This is partially covered by compiler warnings

  // MISRA C:2025 Rule 8.13 - Pointer parameters should be const (Advisory)
  CheckFactories.registerCheck<AvoidNonConstPointerParamCheck>(
      "automotive-c25-adv-8.13");

  // MISRA C:2025 Rule 17.9 - Noreturn function shall not return (Mandatory)
  // Note: This check may be superseded by clang-diagnostic-invalid-noreturn
  CheckFactories.registerCheck<AvoidNoreturnReturnCheck>(
      "automotive-c25-mand-17.9");
}

} // namespace clang::tidy::automotive

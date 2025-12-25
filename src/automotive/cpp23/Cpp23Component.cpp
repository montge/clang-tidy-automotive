//===--- Cpp23Component.cpp - clang-tidy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Cpp23Component.h"
#include "AvoidArrayToPointerDecayCheck.h"
#include "AvoidAssignmentInSubExpressionCheck.h"
#include "AvoidAssignmentResultCheck.h"
#include "AvoidCStyleCastCheck.h"
#include "AvoidCharacterComparisonMismatchCheck.h"
#include "AvoidCharacterTypeMismatchCheck.h"
#include "AvoidConstCastCheck.h"
#include "AvoidConstOnlyOverloadCheck.h"
#include "AvoidConstexprMutableCheck.h"
#include "AvoidCtimeCppCheck.h"
#include "AvoidDanglingReferenceCheck.h"
#include "AvoidDynamicCastCheck.h"
#include "AvoidDynamicExceptionSpecCheck.h"
#include "AvoidDynamicMemoryCppCheck.h"
#include "AvoidExceptionAcrossBoundaryCheck.h"
#include "AvoidExceptionCheck.h"
#include "AvoidFloatingPointEqualityCheck.h"
#include "AvoidFloatingPointLoopCounterCheck.h"
#include "AvoidForLoopVarModificationCheck.h"
#include "AvoidIdentifierShadowingCheck.h"
#include "AvoidImplicitConversionCheck.h"
#include "AvoidImplicitConversionOperatorCheck.h"
#include "AvoidImplicitLambdaCaptureCheck.h"
#include "AvoidInlineTypeDefinitionCheck.h"
#include "AvoidMemberPointerConversionCheck.h"
#include "AvoidModifyingByValueParamCheck.h"
#include "AvoidNarrowingConversionCheck.h"
#include "AvoidNestedSwitchLabelCheck.h"
#include "AvoidNonCompoundBodyCheck.h"
#include "AvoidNoreturnReturnCheck.h"
#include "AvoidNullLiteralAssignmentCheck.h"
#include "AvoidObjectSlicingCheck.h"
#include "AvoidOverlappingCopyCheck.h"
#include "AvoidOverloadedLogicalOperatorCheck.h"
#include "AvoidReallocCheck.h"
#include "AvoidReinterpretCastCheck.h"
#include "AvoidReturnLocalAddressCheck.h"
#include "AvoidSingleClauseSwitchCppCheck.h"
#include "AvoidSlicingCheck.h"
#include "AvoidStatementExpressionCheck.h"
#include "AvoidThrowInNoexceptCheck.h"
#include "AvoidThrowingDestructorCheck.h"
#include "AvoidTypePunningCheck.h"
#include "AvoidUnaryMinusOnUnsignedCheck.h"
#include "AvoidUnboundedLoopCheck.h"
#include "AvoidUninitializedArrayCheck.h"
#include "AvoidUnionCppCheck.h"
#include "AvoidUnionTypePunningCheck.h"
#include "AvoidUnrelatedPointerConversionCheck.h"
#include "AvoidUnsignedWrapAroundCheck.h"
#include "AvoidUnusedVariadicTemplateCheck.h"
#include "AvoidVirtualBaseClassCheck.h"
#include "AvoidVirtualFunctionDefaultArgCheck.h"
#include "AvoidVoidPointerCastCheck.h"
#include "AvoidWideVariableScopeCheck.h"
#include "DefaultArgumentOrderCheck.h"
#include "ExplicitConstructorCheck.h"
#include "ExplicitLambdaCaptureCheck.h"
#include "LocaleConstPointerCheck.h"
#include "MissingReturnCheck.h"
#include "NoexceptMoveCheck.h"
#include "NoreturnVoidReturnCheck.h"
#include "ProperConceptDefinitionCheck.h"
#include "RequireExplicitConversionCheck.h"
#include "RequireVirtualDestructorCheck.h"
#include "RuleOfFiveCheck.h"
#include "SinglePointOfExitCheck.h"
#include "SuboptimalBreakContinueCheck.h"
#include "UnconstrainedTemplateCheck.h"
#include "UninitializedAutoVarCheck.h"
#include "UseAddressofCheck.h"
#include "UseUnnamedNamespaceCheck.h"
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

  // MISRA C++:2023 Rule 12.2.1 - Base class destructor shall be public and
  // virtual, or protected and non-virtual
  CheckFactories
      .registerCheck<::clang::tidy::automotive::RequireVirtualDestructorCheck>(
          "automotive-cpp23-req-12.2.1");

  // MISRA C++:2023 Rule 15.1.3 - Single-argument constructors shall be explicit
  CheckFactories.registerCheck<ExplicitConstructorCheck>(
      "automotive-cpp23-req-15.1.3");

  // MISRA C++:2023 Rule 15.2 - Virtual base classes should be avoided
  CheckFactories.registerCheck<cpp23::AvoidVirtualBaseClassCheck>(
      "automotive-cpp23-adv-15.2");

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

  // MISRA C++:2023 Rule 18.2 - Exceptions shall not propagate out of
  // destructors (Alias for Rule 18.4.1)
  CheckFactories.registerCheck<AvoidThrowingDestructorCheck>(
      "automotive-cpp23-req-18.2");

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

  // MISRA C++:2023 Rule 12.4.1 - Virtual functions shall not have default
  // arguments
  CheckFactories.registerCheck<AvoidVirtualFunctionDefaultArgCheck>(
      "automotive-cpp23-req-12.4.1");

  // MISRA C++:2023 Rule 16.0.1 - Exceptions shall not be thrown across
  // execution boundaries
  CheckFactories.registerCheck<cpp23::AvoidExceptionAcrossBoundaryCheck>(
      "automotive-cpp23-req-16.0.1");

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

  // MISRA C++:2023 Rule 0.1 - Implicit lambda capture
  CheckFactories.registerCheck<AvoidImplicitLambdaCaptureCheck>(
      "automotive-cpp23-req-0.1");

  // MISRA C++:2023 Rule 11.3 - C-style cast prohibition
  // (Alias for existing check, same functionality as Rule 8.2.1)
  CheckFactories.registerCheck<AvoidCStyleCastCheck>(
      "automotive-cpp23-adv-11.3");

  // MISRA C++:2023 Rule 11.3.2 - Overloaded functions should differ in
  // parameter types, not just in parameter const qualification
  CheckFactories.registerCheck<cpp23::AvoidConstOnlyOverloadCheck>(
      "automotive-cpp23-adv-11.3.2");

  // MISRA C++:2023 Rule 9.5 - Switch label in outermost compound statement
  CheckFactories.registerCheck<AvoidNestedSwitchLabelCheck>(
      "automotive-cpp23-req-9.5");
  // MISRA C++:2023 Rule 9.5.1 - For-loop iteration variable modification
  CheckFactories.registerCheck<AvoidForLoopVarModificationCheck>(
      "automotive-cpp23-req-9.5.1");

  // MISRA C++:2023 Rule 13.1 - Copy/move operations should not have overlapping
  // source and destination
  CheckFactories.registerCheck<cpp23::AvoidOverlappingCopyCheck>(
      "automotive-cpp23-req-13.1");

  // MISRA C++:2023 Rule 13.3 - Parameter by value modification
  CheckFactories.registerCheck<AvoidModifyingByValueParamCheck>(
      "automotive-cpp23-req-13.3");

  // MISRA C++:2023 Rule 6.3 - Loop counters should have well-defined bounds
  CheckFactories.registerCheck<cpp23::AvoidUnboundedLoopCheck>(
      "automotive-cpp23-req-6.3");

  // MISRA C++:2023 Rule 6.4 - Assignment in sub-expression
  CheckFactories.registerCheck<AvoidAssignmentInSubExpressionCheck>(
      "automotive-cpp23-req-6.4");

  // MISRA C++:2023 Rule 6.5 - Floating-point loop counter
  CheckFactories.registerCheck<AvoidFloatingPointLoopCounterCheck>(
      "automotive-cpp23-req-6.5");

  // MISRA C++:2023 Rule 9.6 - All return paths
  CheckFactories.registerCheck<MissingReturnCheck>("automotive-cpp23-req-9.6");

  // MISRA C++:2023 Rule 19.3 - realloc prohibition
  CheckFactories.registerCheck<AvoidReallocCheck>("automotive-cpp23-req-19.3");

  // MISRA C++:2023 Rule 9.2 - Auto variable initialization
  CheckFactories.registerCheck<UninitializedAutoVarCheck>(
      "automotive-cpp23-req-9.2");

  // MISRA C++:2023 Rule 7.11 - Null pointer literal assignment
  CheckFactories.registerCheck<AvoidNullLiteralAssignmentCheck>(
      "automotive-cpp23-req-7.11");

  // MISRA C++:2023 Rule 6.2 - Identifier shadowing
  CheckFactories.registerCheck<AvoidIdentifierShadowingCheck>(
      "automotive-cpp23-req-6.2");

  // MISRA C++:2023 Rule 7.0 - Character type assignment
  CheckFactories.registerCheck<cpp23::AvoidCharacterTypeMismatchCheck>(
      "automotive-cpp23-req-7.0");

  // MISRA C++:2023 Rule 4.1 - Character comparison type mismatch
  CheckFactories.registerCheck<cpp23::AvoidCharacterComparisonMismatchCheck>(
      "automotive-cpp23-adv-4.1");

  // MISRA C++:2023 Rule 6.0 - Uninitialized array
  CheckFactories.registerCheck<cpp23::AvoidUninitializedArrayCheck>(
      "automotive-cpp23-adv-6.0");

  // MISRA C++:2023 Rule 6.0.4 - Unary minus on unsigned type
  CheckFactories.registerCheck<AvoidUnaryMinusOnUnsignedCheck>(
      "automotive-cpp23-req-6.0.4");

  // MISRA C++:2023 Rule 16.5 - Overloaded logical operators
  CheckFactories.registerCheck<cpp23::AvoidOverloadedLogicalOperatorCheck>(
      "automotive-cpp23-adv-16.5");

  // MISRA C++:2023 Rule 16.6.1 - Variadic template shall have at least one
  // instantiation with variadic arguments
  CheckFactories.registerCheck<cpp23::AvoidUnusedVariadicTemplateCheck>(
      "automotive-cpp23-adv-16.6.1");

  // MISRA C++:2023 Rule 8.0 - Single point of exit
  CheckFactories.registerCheck<cpp23::SinglePointOfExitCheck>(
      "automotive-cpp23-adv-8.0");

  // MISRA C++:2023 Rule 8.1 - Explicit lambda capture
  CheckFactories.registerCheck<cpp23::ExplicitLambdaCaptureCheck>(
      "automotive-cpp23-adv-8.1");

  // MISRA C++:2023 Rule 18.5 - Dynamic exception specification
  CheckFactories.registerCheck<cpp23::AvoidDynamicExceptionSpecCheck>(
      "automotive-cpp23-adv-18.5");

  // MISRA C++:2023 Rule 19.0 - Use std::addressof
  CheckFactories.registerCheck<cpp23::UseAddressofCheck>(
      "automotive-cpp23-adv-19.0");

  // MISRA C++:2023 Rule 19.1 - Type-punning through union members
  CheckFactories.registerCheck<cpp23::AvoidUnionTypePunningCheck>(
      "automotive-cpp23-req-19.1");

  // MISRA C++:2023 Rule 10.3 - Unnamed namespace for internal linkage
  CheckFactories.registerCheck<cpp23::UseUnnamedNamespaceCheck>(
      "automotive-cpp23-adv-10.3");

  // MISRA C++:2023 Rule 5.0 - Inline type definition
  CheckFactories.registerCheck<cpp23::AvoidInlineTypeDefinitionCheck>(
      "automotive-cpp23-req-5.0");

  // MISRA C++:2023 Rule 6.8 - Suboptimal break/continue placement
  CheckFactories.registerCheck<cpp23::SuboptimalBreakContinueCheck>(
      "automotive-cpp23-adv-6.8");

  // MISRA C++:2023 Rule 7.2 - Unsigned integer operations shall not wrap around
  CheckFactories.registerCheck<cpp23::AvoidUnsignedWrapAroundCheck>(
      "automotive-cpp23-req-7.2");

  // MISRA C++:2023 Rule 7.4 - The result of an assignment shall not be used
  CheckFactories.registerCheck<cpp23::AvoidAssignmentResultCheck>(
      "automotive-cpp23-req-7.4");

  // MISRA C++:2023 Rule 8.19 - Noreturn functions shall not return
  CheckFactories.registerCheck<AvoidNoreturnReturnCheck>(
      "automotive-cpp23-adv-8.19");

  // MISRA C++:2023 Rule 8.20 - Noreturn functions should have void return type
  CheckFactories.registerCheck<NoreturnVoidReturnCheck>(
      "automotive-cpp23-adv-8.20");

  // MISRA C++:2023 Rule 8.18 - Statement expression extension prohibition
  CheckFactories.registerCheck<cpp23::AvoidStatementExpressionCheck>(
      "automotive-cpp23-adv-8.18");

  // MISRA C++:2023 Rule 6.8.2 - Return of pointer/reference to local
  // (Mandatory)
  CheckFactories.registerCheck<cpp23::AvoidReturnLocalAddressCheck>(
      "automotive-cpp23-mand-6.8.2");

  // MISRA C++:2023 Rule 7.5.1 - A function shall not return a reference or a
  // pointer to an automatic variable (Required)
  CheckFactories.registerCheck<cpp23::AvoidDanglingReferenceCheck>(
      "automotive-cpp23-req-7.5.1");

  // MISRA C++:2023 Rule 11.6 - Invalid member pointer conversions
  CheckFactories.registerCheck<cpp23::AvoidMemberPointerConversionCheck>(
      "automotive-cpp23-req-11.6");

  // MISRA C++:2023 Rule 14.1 - Constexpr variables should not have mutable
  // sub-objects
  CheckFactories.registerCheck<cpp23::AvoidConstexprMutableCheck>(
      "automotive-cpp23-adv-14.1");

  // MISRA C++:2023 Rule 5.7 - Variables should be declared in the smallest
  // scope possible
  CheckFactories.registerCheck<cpp23::AvoidWideVariableScopeCheck>(
      "automotive-cpp23-req-5.7");

  // MISRA C++:2023 Rule 5.13 - Type punning prevention
  CheckFactories.registerCheck<cpp23::AvoidTypePunningCheck>(
      "automotive-cpp23-req-5.13");

  // MISRA C++:2023 Rule 10.1 - Pointer conversions between classes not in same
  // hierarchy
  CheckFactories.registerCheck<cpp23::AvoidUnrelatedPointerConversionCheck>(
      "automotive-cpp23-req-10.1");

  // MISRA C++:2023 Rule 10.2 - Conversions from void* to typed pointer
  CheckFactories.registerCheck<cpp23::AvoidVoidPointerCastCheck>(
      "automotive-cpp23-req-10.2");

  // MISRA C++:2023 Rule 10.4.1 - A conversion function shall be explicit
  CheckFactories.registerCheck<cpp23::RequireExplicitConversionCheck>(
      "automotive-cpp23-adv-10.4.1");

  // MISRA C++:2023 Rule 11.0.1 - Object slicing shall not occur
  CheckFactories.registerCheck<cpp23::AvoidObjectSlicingCheck>(
      "automotive-cpp23-req-11.0.1");

  // MISRA C++:2023 Rule 8.5.1 - Default argument ordering and consistency
  CheckFactories.registerCheck<cpp23::DefaultArgumentOrderCheck>(
      "automotive-cpp23-req-8.5.1");

  // MISRA C++:2023 Rule 25.5 - Pointers returned by localeconv, getenv,
  // setlocale, strerror must be const-qualified (Mandatory)
  CheckFactories.registerCheck<cpp23::LocaleConstPointerCheck>(
      "automotive-cpp23-mand-25.5");

  // MISRA C++:2023 Rule 5.10 - Array to pointer decay should be avoided
  CheckFactories.registerCheck<cpp23::AvoidArrayToPointerDecayCheck>(
      "automotive-cpp23-req-5.10");
}

} // namespace clang::tidy::automotive

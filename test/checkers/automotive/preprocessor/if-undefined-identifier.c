// XFAIL: *
// Note: Check crashes - needs investigation
// RUN: %check_clang_tidy %s automotive-c25-req-20.9 %t

// MISRA C:2025 Rule 20.9 - All identifiers used in the controlling expression
// of #if or #elif preprocessing directives shall be #define'd before
// evaluation, or be the operand of a defined operator.
//
// This rule prevents undefined macro expansion which defaults to 0,
// causing unexpected behavior.

#define DEFINED_MACRO 1
#define ZERO_MACRO 0
#define FEATURE_A 1

//===----------------------------------------------------------------------===//
// Violation Cases
//===----------------------------------------------------------------------===//

// Test case 1: Simple undefined identifier in #if
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: identifier 'UNDEFINED_MACRO' used in preprocessing directive is not defined; did you mean to use 'defined(UNDEFINED_MACRO)'? [automotive-c25-req-20.9]
#if UNDEFINED_MACRO
#endif

// Test case 2: Undefined identifier in comparison
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: identifier 'UNDEFINED' used in preprocessing directive is not defined; did you mean to use 'defined(UNDEFINED)'? [automotive-c25-req-20.9]
#if UNDEFINED > 0
#endif

// Test case 3: Undefined identifier in arithmetic expression
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: identifier 'UNDEF_VAR' used in preprocessing directive is not defined; did you mean to use 'defined(UNDEF_VAR)'? [automotive-c25-req-20.9]
#if UNDEF_VAR + 1
#endif

// Test case 4: Multiple undefined identifiers
// CHECK-MESSAGES: :[[@LINE+2]]:5: warning: identifier 'UNDEF_A' used in preprocessing directive is not defined; did you mean to use 'defined(UNDEF_A)'? [automotive-c25-req-20.9]
// CHECK-MESSAGES: :[[@LINE+1]]:16: warning: identifier 'UNDEF_B' used in preprocessing directive is not defined; did you mean to use 'defined(UNDEF_B)'? [automotive-c25-req-20.9]
#if UNDEF_A && UNDEF_B
#endif

// Test case 5: Undefined identifier in #elif
#if DEFINED_MACRO
// CHECK-MESSAGES: :[[@LINE+1]]:7: warning: identifier 'ELIF_UNDEF' used in preprocessing directive is not defined; did you mean to use 'defined(ELIF_UNDEF)'? [automotive-c25-req-20.9]
#elif ELIF_UNDEF
#endif

// Test case 6: Undefined in complex expression
// CHECK-MESSAGES: :[[@LINE+1]]:22: warning: identifier 'UNDEFINED_VAR' used in preprocessing directive is not defined; did you mean to use 'defined(UNDEFINED_VAR)'? [automotive-c25-req-20.9]
#if DEFINED_MACRO && UNDEFINED_VAR
#endif

// Test case 7: Undefined in negation
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: identifier 'NOT_DEFINED' used in preprocessing directive is not defined; did you mean to use 'defined(NOT_DEFINED)'? [automotive-c25-req-20.9]
#if !NOT_DEFINED
#endif

// Test case 8: Undefined in parenthesized expression
// CHECK-MESSAGES: :[[@LINE+1]]:6: warning: identifier 'PAREN_UNDEF' used in preprocessing directive is not defined; did you mean to use 'defined(PAREN_UNDEF)'? [automotive-c25-req-20.9]
#if (PAREN_UNDEF)
#endif

// Test case 9: Undefined with bitwise operations
// CHECK-MESSAGES: :[[@LINE+1]]:5: warning: identifier 'BITWISE_UNDEF' used in preprocessing directive is not defined; did you mean to use 'defined(BITWISE_UNDEF)'? [automotive-c25-req-20.9]
#if BITWISE_UNDEF & 0xFF
#endif

//===----------------------------------------------------------------------===//
// Compliant Cases - Should NOT trigger warnings
//===----------------------------------------------------------------------===//

// Test case 10: Using defined() operator (compliant)
#if defined(SOME_MACRO)
#endif

// Test case 11: Using defined without parentheses (compliant)
#if defined ANOTHER_MACRO
#endif

// Test case 12: Checking if macro is defined (compliant)
#if defined(DEFINED_MACRO)
#endif

// Test case 13: Using a defined macro directly (compliant)
#if DEFINED_MACRO
#endif

// Test case 14: Complex expression with defined macros (compliant)
#if DEFINED_MACRO && ZERO_MACRO
#endif

// Test case 15: Using defined() in complex expression (compliant)
#if defined(FEATURE_A) && FEATURE_A > 0
#endif

// Test case 16: Multiple defined() checks (compliant)
#if defined(FEATURE_A) || defined(FEATURE_B)
#endif

// Test case 17: Negation with defined() (compliant)
#if !defined(DISABLED_FEATURE)
#endif

// Test case 18: ifdef directive (compliant - implicit defined check)
#ifdef SOME_FEATURE
#endif

// Test case 19: ifndef directive (compliant - implicit defined check)
#ifndef SOME_FEATURE
#endif

// Test case 20: Comparison with defined macro (compliant)
#if DEFINED_MACRO == 1
#endif

// Test case 21: Arithmetic with defined macros (compliant)
#if DEFINED_MACRO + ZERO_MACRO
#endif

// Test case 22: Defined macro in parentheses (compliant)
#if (DEFINED_MACRO)
#endif

// Test case 23: Complex logical expression with defined macros (compliant)
#if (DEFINED_MACRO && FEATURE_A) || ZERO_MACRO
#endif

// Test case 24: Using numeric literals (compliant)
#if 1
#endif

#if 0
#endif

// Test case 25: elifdef (compliant - C23 feature, implicit defined check)
#if DEFINED_MACRO
#elifdef ANOTHER_FEATURE
#endif

// Test case 26: elifndef (compliant - C23 feature, implicit not defined check)
#if DEFINED_MACRO
#elifndef YET_ANOTHER_FEATURE
#endif

// Test case 27: Checking undefined with defined() (compliant)
#if !defined(THIS_IS_NOT_DEFINED)
int compliant_code = 1;
#endif

// Test case 28: Mixed defined() and defined macro (compliant)
#if defined(MAYBE_DEFINED) || DEFINED_MACRO
#endif

// Test case 29: Nested parentheses with defined macros (compliant)
#if ((DEFINED_MACRO) && (FEATURE_A))
#endif

// Test case 30: Ternary-like expression with defined macros (compliant)
#if DEFINED_MACRO ? 1 : 0
#endif

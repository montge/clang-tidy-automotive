// RUN: %check_clang_tidy %s automotive-c25-req-20.8 %t

// MISRA C:2025 Rule 20.8 - The controlling expression of a #if or #elif
// preprocessing directive shall evaluate to 0 or 1.
//
// This rule ensures that preprocessor conditions use boolean-like expressions,
// improving code clarity and avoiding unexpected behavior from non-boolean
// integer values.

// Define some test macros
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FEATURE_ENABLED 1
#define FEATURE_DISABLED 0
#define VERSION_MAJOR 5

//===----------------------------------------------------------------------===//
// Compliant Cases - Should NOT trigger warnings
//===----------------------------------------------------------------------===//

// Literal 0 and 1 are compliant
#if 0
int compliant1;
#endif

#if 1
int compliant2;
#endif

// defined() always evaluates to 0 or 1
#if defined(ZERO)
int compliant3;
#endif

#if defined(UNDEFINED_MACRO)
int compliant4;
#endif

// Logical NOT operator yields 0 or 1
#if !ZERO
int compliant5;
#endif

#if !FEATURE_ENABLED
int compliant6;
#endif

// Comparison operators yield 0 or 1
#if VERSION_MAJOR > 3
int compliant7;
#endif

#if VERSION_MAJOR >= 5
int compliant8;
#endif

#if VERSION_MAJOR < 10
int compliant9;
#endif

#if VERSION_MAJOR <= 5
int compliant10;
#endif

#if VERSION_MAJOR == 5
int compliant11;
#endif

#if VERSION_MAJOR != 3
int compliant12;
#endif

// Logical AND operator yields 0 or 1
#if FEATURE_ENABLED && defined(ZERO)
int compliant13;
#endif

// Logical OR operator yields 0 or 1
#if FEATURE_DISABLED || FEATURE_ENABLED
int compliant14;
#endif

// Complex boolean expressions
#if (VERSION_MAJOR > 3) && (VERSION_MAJOR < 10)
int compliant15;
#endif

#if defined(ZERO) || defined(ONE)
int compliant16;
#endif

// Single identifier macros (conservative: allowed)
#if FEATURE_ENABLED
int compliant17;
#endif

#if FEATURE_DISABLED
int compliant18;
#endif

#if ZERO
int compliant19;
#endif

#if ONE
int compliant20;
#endif

// elif with compliant expressions
#if 0
int dummy1;
#elif 1
int compliant21;
#elif defined(ZERO)
int compliant22;
#elif FEATURE_ENABLED > 0
int compliant23;
#endif

//===----------------------------------------------------------------------===//
// Violation Cases - Should trigger warnings
//===----------------------------------------------------------------------===//

// Literal values other than 0 or 1
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if 2
int violation1;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if 3
int violation2;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if 10
int violation3;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if -1
int violation4;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if 0xFF
int violation5;
#endif

// Arithmetic expressions (result may not be 0 or 1)
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if VERSION_MAJOR + 1
int violation6;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if VERSION_MAJOR - 1
int violation7;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if VERSION_MAJOR * 2
int violation8;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if VERSION_MAJOR / 2
int violation9;
#endif

// Bitwise operations (except for single identifier used in boolean context)
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if VERSION_MAJOR & 0x01
int violation10;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if VERSION_MAJOR | 0x02
int violation11;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if VERSION_MAJOR ^ 0x01
int violation12;
#endif

// Left/right shift operations
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if 1 << 2
int violation13;
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if 8 >> 1
int violation14;
#endif

// Conditional operator (ternary) - result depends on values
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if FEATURE_ENABLED ? 5 : 3
int violation15;
#endif

// elif violations
#if 0
int dummy2;
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #elif directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#elif 2
int violation16;
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #elif directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#elif VERSION_MAJOR + 1
int violation17;
#endif

// Complex expressions with arithmetic
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if (VERSION_MAJOR + TWO) * THREE
int violation18;
#endif

// Modulo operation
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if VERSION_MAJOR % 2
int violation19;
#endif

// Bitwise NOT (result depends on the value)
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #if directive condition shall evaluate to 0 or 1 [automotive-c25-req-20.8]
#if ~ZERO
int violation20;
#endif

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

// Nested parentheses with compliant expressions are OK
#if ((FEATURE_ENABLED == 1))
int edge_compliant1;
#endif

// Mixed compliant expressions
#if (defined(ZERO) && (VERSION_MAJOR > 0))
int edge_compliant2;
#endif

// Multiple comparisons
#if VERSION_MAJOR > 0 && VERSION_MAJOR < 100
int edge_compliant3;
#endif

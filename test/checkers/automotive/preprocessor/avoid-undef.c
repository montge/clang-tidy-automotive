// Test file for: automotive-avoid-undef
// Related MISRA C:2025 Directive: 4.5
//
// This file tests the detection of #undef directives

// RUN: %check_clang_tidy %s automotive-avoid-undef %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Define a macro then undefine it
#define TEST_MACRO 1

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef TEST_MACRO

// Define and immediately undefine
#define TEMP_MACRO 42
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef TEMP_MACRO

// Undefine a function-like macro
#define FUNC_MACRO(x) ((x) * 2)
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef FUNC_MACRO

// Undefine a string macro
#define VERSION "1.0.0"
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef VERSION

// Multiple undefs
#define MACRO_A 10
#define MACRO_B 20
#define MACRO_C 30

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef MACRO_A
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef MACRO_B
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef MACRO_C

// Undef in conditional compilation
#define FEATURE_ENABLED 1

#if FEATURE_ENABLED
    // Some code
#endif

// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef FEATURE_ENABLED

// Undef and redefine pattern (still violates)
#define CONFIG_VALUE 100
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef CONFIG_VALUE
#define CONFIG_VALUE 200

// Undef a previously undefined macro (to ensure it's not defined)
#define ENSURE_UNDEFINED 1
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef ENSURE_UNDEFINED

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Define macros without undefining them
#define MAX_SIZE 256
#define MIN_SIZE 16
#define DEFAULT_VALUE 0

// Function-like macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// String macros
#define APP_NAME "MyApp"
#define APP_VERSION "2.0"

// Multi-line macros
#define COMPLEX_MACRO(x, y) \
    do { \
        int temp = (x) + (y); \
        temp = temp * 2; \
    } while (0)

// Conditional compilation without undef
#define PLATFORM_LINUX 1

#if PLATFORM_LINUX
    #define USE_POSIX 1
#else
    #define USE_WIN32 1
#endif

// Include guards (no undef)
#ifndef MY_HEADER_H
#define MY_HEADER_H
    // Header contents
#endif

// Macro definitions for conditional features
#define FEATURE_X_ENABLED 1
#define FEATURE_Y_ENABLED 0

#if FEATURE_X_ENABLED
    // Feature X code
#endif

#if FEATURE_Y_ENABLED
    // Feature Y code
#endif

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

// Attempting to undef an undefined macro (doesn't trigger in this implementation)
#undef NEVER_DEFINED_MACRO

// Undef in nested conditional compilation
#define NESTED_FEATURE 1

#ifdef NESTED_FEATURE
    #define INNER_MACRO 100
    #ifdef INNER_MACRO
        // Code
    #endif
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid #undef [automotive-avoid-undef]
    #undef INNER_MACRO
#endif

// Undef after multiple defines with same name (redefinition)
#define REDEF_MACRO 1
// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: 'REDEF_MACRO' macro redefined [clang-diagnostic-macro-redefined]
#define REDEF_MACRO 2
// CHECK-MESSAGES: :137:9: note: previous definition is here
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef REDEF_MACRO

// Comment before undef (doesn't make it compliant)
#define COMMENTED_MACRO 42
// Undefining this macro because...
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef COMMENTED_MACRO

// Undef in different contexts
#define GLOBAL_MACRO 1

void test_function(void) {
    // Note: #undef can't actually appear inside a function in C,
    // but this tests file-level scope
}

// This undef is at file scope
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef GLOBAL_MACRO

// Typical include guard pattern (compliant - no undef)
#ifndef INCLUDE_GUARD_H
#define INCLUDE_GUARD_H

// Header content

#endif // INCLUDE_GUARD_H

// Macro used for version checking
#define API_VERSION_MAJOR 2
#define API_VERSION_MINOR 5
#define API_VERSION_PATCH 0

// Calculate composite version
#define API_VERSION ((API_VERSION_MAJOR * 10000) + (API_VERSION_MINOR * 100) + API_VERSION_PATCH)

// Undef one of the version components (violation)
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef API_VERSION_PATCH

// Boolean flag macros
#define DEBUG_MODE 1
#define RELEASE_MODE 0

#if DEBUG_MODE
    #define LOG_LEVEL 3
#else
    #define LOG_LEVEL 1
#endif

// Platform-specific macros
#if defined(__linux__)
    #define PLATFORM_NAME "Linux"
#elif defined(_WIN32)
    #define PLATFORM_NAME "Windows"
#else
    #define PLATFORM_NAME "Unknown"
#endif

// Empty macro
#define EMPTY_MACRO
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef EMPTY_MACRO

// Macro with no replacement text
#define FLAG
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef FLAG

// Test file for: automotive-avoid-hash-operator
// Related MISRA C:2025 Directive: 4.12
//
// This file tests the detection of # (stringification) and ## (token pasting) operators in macros

// RUN: %check_clang_tidy %s automotive-avoid-hash-operator %t

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Single # operator (stringification)
// CHECK-MESSAGES: :[[@LINE+1]]:29: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define STRINGIFY(x) #x

// Token pasting operator ##
// CHECK-MESSAGES: :[[@LINE+1]]:26: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define CONCAT(a, b) a##b

// Multiple # operators
// CHECK-MESSAGES: :[[@LINE+2]]:35: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+1]]:44: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define STRINGIFY_TWO(x, y) #x, #y

// Multiple ## operators
// CHECK-MESSAGES: :[[@LINE+2]]:32: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+1]]:40: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define CONCAT_THREE(a, b, c) a##b##c

// # in function-like macro
// CHECK-MESSAGES: :[[@LINE+1]]:44: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define MAKE_STRING_LITERAL(name) #name

// ## in function-like macro
// CHECK-MESSAGES: :[[@LINE+1]]:42: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define MAKE_IDENTIFIER(prefix, suffix) prefix##suffix

// Complex macro with both # and ##
// CHECK-MESSAGES: :[[@LINE+2]]:38: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+1]]:48: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define COMPLEX(prefix, name) prefix##name, #name

// # operator for creating format strings
// CHECK-MESSAGES: :[[@LINE+1]]:45: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define LOG_VALUE(x) printf("Value: " #x "\n")

// ## for creating variable names
// CHECK-MESSAGES: :[[@LINE+1]]:42: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define DECLARE_VAR(type, name) type var_##name

// Stringification with formatting
// CHECK-MESSAGES: :[[@LINE+1]]:48: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define DEBUG_PRINT(x) fprintf(stderr, #x " = %d\n", x)

// Token pasting for function names
// CHECK-MESSAGES: :[[@LINE+1]]:43: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define MAKE_FUNC_NAME(prefix) prefix##_function

// # in multi-line macro
// CHECK-MESSAGES: :[[@LINE+3]]:9: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define MULTI_LINE_STRINGIFY(x) \
    do { \
        const char* str = #x; \
    } while (0)

// ## in multi-line macro
// CHECK-MESSAGES: :[[@LINE+3]]:27: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define MULTI_LINE_CONCAT(a, b) \
    do { \
        int combined = a##b; \
    } while (0)

// Nested usage (still uses the operators)
// CHECK-MESSAGES: :[[@LINE+1]]:37: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define OUTER_STRINGIFY(x) #x
#define INNER_VALUE 123
#define RESULT OUTER_STRINGIFY(INNER_VALUE)

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Simple macros without # or ##
#define MAX_SIZE 256
#define MIN_SIZE 16
#define PI 3.14159

// Function-like macros without operators
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

// Macros with expressions
#define ADD(a, b) ((a) + (b))
#define MULTIPLY(a, b) ((a) * (b))

// Conditional macros
#define IS_POSITIVE(x) ((x) > 0)
#define IS_ZERO(x) ((x) == 0)

// Macros with string literals (not stringification)
#define ERROR_MSG "An error occurred"
#define VERSION_STRING "1.0.0"

// Multi-line macro without # or ##
#define SWAP(a, b) \
    do { \
        typeof(a) temp = (a); \
        (a) = (b); \
        (b) = temp; \
    } while (0)

// Macro with conditional compilation
#if defined(DEBUG)
    #define DEBUG_ENABLED 1
#else
    #define DEBUG_ENABLED 0
#endif

// Function-like macro with multiple parameters
#define CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

// Macro with cast
#define TO_INT(x) ((int)(x))
#define TO_FLOAT(x) ((float)(x))

// Macro with sizeof
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

// Bit manipulation macros
#define BIT_SET(val, bit) ((val) | (1 << (bit)))
#define BIT_CLEAR(val, bit) ((val) & ~(1 << (bit)))
#define BIT_CHECK(val, bit) ((val) & (1 << (bit)))

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

// Empty macro (no operators)
#define EMPTY

// Macro with only one token
#define SINGLE_TOKEN value

// Variadic macro without # or ##
#define PRINT_VALUES(...) printf(__VA_ARGS__)

// Variadic macro with # (violation)
// CHECK-MESSAGES: :[[@LINE+1]]:39: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define STRINGIFY_VA(...) #__VA_ARGS__

// Variadic macro with ## (violation)
// CHECK-MESSAGES: :[[@LINE+1]]:44: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define CONCAT_VA(prefix, ...) prefix##__VA_ARGS__

// Macro using # in comment (should not warn - comments are removed)
#define COMMENTED_MACRO(x) ((x) * 2) /* Not using # operator */

// Macro using ## in comment (should not warn)
#define ANOTHER_COMMENTED(x) ((x) + 1) /* Not using ## operator */

// String literal containing # (not the operator)
#define HASH_STRING "Use # for comments in some languages"

// Character literal containing # (rare but valid)
#define HASH_CHAR '#'

// Conditional macro expansion
#define CONDITIONAL_VALUE(x) \
    _Generic((x), \
        int: (x), \
        float: (x), \
        default: 0)

// Macro with parentheses and no operators
#define CALL_FUNC(f, x) f((x))

// Macro for include guards (no operators)
#ifndef MY_HEADER_H
#define MY_HEADER_H
// Header content
#endif

// Macro with backslash continuation but no operators
#define LONG_EXPRESSION(a, b, c) \
    ((a) + (b) * (c) - \
     ((a) * (b)) + \
     ((b) * (c)))

// X-Macro pattern using # and ## (violations)
// CHECK-MESSAGES: :[[@LINE+1]]:30: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define X_ENUM(name) #name,

// CHECK-MESSAGES: :[[@LINE+1]]:35: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define X_FUNC(name) void func_##name(void);

// Common assertion macro pattern with #
// CHECK-MESSAGES: :[[@LINE+1]]:63: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define ASSERT(cond) if (!(cond)) { fprintf(stderr, "Failed: " #cond); }

// Common pattern for creating unique identifiers with ##
// CHECK-MESSAGES: :[[@LINE+1]]:43: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define UNIQUE_VAR(name) var_##name##_unique

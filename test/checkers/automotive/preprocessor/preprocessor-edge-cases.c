// Test file for: automotive preprocessor checks (Edge Cases)
// Related MISRA C:2025 Rules: 20.4, 20.5, 20.10, 20.11
//
// This file tests edge cases for preprocessor-related checks

// RUN: %check_clang_tidy %s automotive-avoid-macro-named-as-ckeyword,automotive-avoid-undef,automotive-avoid-hash-operator,automotive-avoid-multiple-hash-operators %t

//===----------------------------------------------------------------------===//
// Edge Case: Macro named as keyword (20.4)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro named as C keyword
#define if while

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro named as C keyword
#define return goto

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro named as C keyword
#define NULL ((void*)0)  // Redefining standard macro

// Compliant: Not a keyword
#define IF_CONDITION(x) (x)
#define RETURN_VALUE 42

//===----------------------------------------------------------------------===//
// Edge Case: #undef usage (20.5)
//===----------------------------------------------------------------------===//

#define TEMP_VALUE 100
// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #undef should not be used
#undef TEMP_VALUE

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: #undef should not be used
#undef UNDEFINED_MACRO  // Undefining non-existent macro

//===----------------------------------------------------------------------===//
// Edge Case: # operator (20.10)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: # operator should not be used
#define STRINGIFY(x) #x

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: # operator should not be used
#define TO_STRING(x) STRINGIFY(x)

//===----------------------------------------------------------------------===//
// Edge Case: ## operator (20.10)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: ## operator should not be used
#define CONCAT(a, b) a##b

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: ## operator should not be used
#define MAKE_VAR(prefix, num) prefix##_##num

//===----------------------------------------------------------------------===//
// Edge Case: Multiple # or ## operators (20.11)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: multiple # or ## operators
#define MULTI_CONCAT(a, b, c) a##b##c

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: multiple # or ## operators
#define MULTI_STRINGIFY(a, b) #a " " #b

//===----------------------------------------------------------------------===//
// Edge Case: Nested macro with operators
//===----------------------------------------------------------------------===//

#define OUTER(x) STRINGIFY(x)  // Uses # through STRINGIFY

//===----------------------------------------------------------------------===//
// Compliant Cases
//===----------------------------------------------------------------------===//

// Simple value macros
#define MAX_SIZE 100
#define VERSION "1.0.0"

// Function-like macros without # or ##
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

// Conditional compilation
#ifdef DEBUG
#define LOG(msg) printf("%s\n", msg)
#else
#define LOG(msg) ((void)0)
#endif

// Include guards (not keywords)
#ifndef HEADER_GUARD_H
#define HEADER_GUARD_H
#endif

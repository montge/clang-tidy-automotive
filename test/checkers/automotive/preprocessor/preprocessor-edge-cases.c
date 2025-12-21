// Test file for: automotive preprocessor checks (Edge Cases)
// Related MISRA C:2025 Rules: 20.4, 20.5, 20.10, 20.11
//
// This file tests edge cases for preprocessor-related checks

// RUN: %check_clang_tidy %s automotive-avoid-macro-named-as-ckeyword,automotive-avoid-undef,automotive-avoid-hash-operator,automotive-avoid-multiple-hash-operators %t

//===----------------------------------------------------------------------===//
// Edge Case: Macro named as keyword (20.4)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name 'if' conflicts with C keyword [automotive-avoid-macro-named-as-ckeyword]
#define if while

// CHECK-MESSAGES: :[[@LINE+1]]:9: warning: macro name 'return' conflicts with C keyword [automotive-avoid-macro-named-as-ckeyword]
#define return goto

// Compliant: Not a keyword
#define IF_CONDITION(x) (x)
#define RETURN_VALUE 42

//===----------------------------------------------------------------------===//
// Edge Case: #undef usage (20.5)
//===----------------------------------------------------------------------===//

#define TEMP_VALUE 100
// CHECK-MESSAGES: :[[@LINE+1]]:8: warning: avoid #undef [automotive-avoid-undef]
#undef TEMP_VALUE

// Note: #undef of non-existent macro doesn't trigger in this implementation
#undef UNDEFINED_MACRO

//===----------------------------------------------------------------------===//
// Edge Case: # operator (20.10)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:22: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
#define STRINGIFY(x) #x

#define TO_STRING(x) STRINGIFY(x)

//===----------------------------------------------------------------------===//
// Edge Case: ## operator (20.10)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+1]]:23: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
#define CONCAT(a, b) a##b

// CHECK-MESSAGES: :[[@LINE+4]]:37: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+3]]:40: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+2]]:40: warning: avoid '##' operator after '##' operator [automotive-avoid-multiple-hash-operators]
// CHECK-MESSAGES: :[[@LINE+1]]:37: note: location of '##' operator
#define MAKE_VAR(prefix, num) prefix##_##num

//===----------------------------------------------------------------------===//
// Edge Case: Multiple # or ## operators (20.11)
//===----------------------------------------------------------------------===//

// CHECK-MESSAGES: :[[@LINE+4]]:27: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+3]]:30: warning: avoid preprocessor operator '##' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+2]]:30: warning: avoid '##' operator after '##' operator [automotive-avoid-multiple-hash-operators]
// CHECK-MESSAGES: :[[@LINE+1]]:27: note: location of '##' operator
#define CONCAT3(a, b, c) a##b##c

// CHECK-MESSAGES: :[[@LINE+4]]:30: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+3]]:37: warning: avoid preprocessor operator '#' [automotive-avoid-hash-operator]
// CHECK-MESSAGES: :[[@LINE+2]]:37: warning: avoid '#' operator after '#' operator [automotive-avoid-multiple-hash-operators]
// CHECK-MESSAGES: :[[@LINE+1]]:30: note: location of '#' operator
#define STRINGIFY_BOTH(x, y) #x "," #y

// Compliant macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

void test_function(void) {
    int x = SQUARE(5);
    (void)x;
}

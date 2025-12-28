// XFAIL: *
// Test file for: automotive-generic-macro-nesting
// A function-like macro shall not invoke another function-like macro with generic type parameter
//
// RUN: %check_clang_tidy %s automotive-generic-macro-nesting %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Compliant: direct generic selection, no nesting
#define TYPE_ID(x) _Generic((x), int: 1, float: 2, default: 0)

void test_direct_macro(void) {
    int x = 5;
    int result = TYPE_ID(x);
    (void)result;
}

// Compliant: non-generic macro calling generic macro
#define CONSTANT 42
#define USE_CONSTANT() TYPE_ID(CONSTANT)

void test_non_generic_call(void) {
    int result = USE_CONSTANT();
    (void)result;
}

// Compliant: inline generic selection without macros
void test_inline_generic(void) {
    int x = 10;
    int result = _Generic(x, int: 1, float: 2, default: 0);
    (void)result;
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Non-compliant: WRAPPER calls TYPE_ID with parameter x
#define INNER_GENERIC(x) _Generic((x), int: 1, float: 2)
#define OUTER_WRAPPER(x) INNER_GENERIC(x)

void test_nested_generic_param(void) {
    int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: generic selection appears to be in a nested macro expansion; function-like macros should not invoke other macros with generic type parameters [automotive-generic-macro-nesting]
    int result = OUTER_WRAPPER(x);
    (void)result;
}

// Non-compliant: more complex nesting
#define SELECT_TYPE(x) _Generic((x), int: process_int(x), float: process_float(x))
#define PROCESS_WRAPPER(y) SELECT_TYPE(y)

void process_int(int x) { (void)x; }
void process_float(float x) { (void)x; }

void test_complex_nesting(void) {
    int val = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: generic selection appears to be in a nested macro expansion; function-like macros should not invoke other macros with generic type parameters [automotive-generic-macro-nesting]
    PROCESS_WRAPPER(val);
}

// Non-compliant: multiple levels of nesting
#define LEVEL1(x) _Generic((x), int: 1, float: 2)
#define LEVEL2(x) LEVEL1(x)
#define LEVEL3(x) LEVEL2(x)

void test_multi_level_nesting(void) {
    int x = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: generic selection appears to be in a nested macro expansion; function-like macros should not invoke other macros with generic type parameters [automotive-generic-macro-nesting]
    int result = LEVEL3(x);
    (void)result;
}

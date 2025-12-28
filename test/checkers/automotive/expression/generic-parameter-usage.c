// XFAIL: *
// Test file for: automotive-generic-parameter-usage
// Generic selection parameters shall be used at most once
//
// RUN: %check_clang_tidy %s automotive-generic-parameter-usage %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void process(int x) { (void)x; }

// Compliant: parameter used once in each association
#define SINGLE_USE(x) _Generic((x), \
    int: process(x), \
    float: (x))

void test_single_use(void) {
    int i = 10;
    float f = 5.5f;

    SINGLE_USE(i);
    SINGLE_USE(f);
}

// Compliant: controlling expression uses parameter once
#define TYPE_CHECK(x) _Generic((x), int: 1, float: 2, default: 0)

void test_controlling_once(void) {
    int x = 5;
    int result = TYPE_CHECK(x);
    (void)result;
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Non-compliant: controlling expression uses x twice
#define DOUBLE_CONTROL(x) _Generic((x) + (x), int: 1, float: 2)

void test_double_in_control(void) {
    int i = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:47: warning: generic selection controlling expression uses parameter 'i' 2 times; parameters should be used at most once [automotive-generic-parameter-usage]
    int result = DOUBLE_CONTROL(i);
    (void)result;
}

// Non-compliant: association expression uses x twice
#define DOUBLE_ASSOC(x) _Generic((x), int: (x) + (x), float: (x))

void test_double_in_assoc(void) {
    int i = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:47: warning: generic association expression uses parameter 'i' 2 times; parameters should be used at most once [automotive-generic-parameter-usage]
    int result = DOUBLE_ASSOC(i);
    (void)result;
}

// Non-compliant: sizeof used multiple times
#define SIZE_DOUBLE(x) _Generic((x), int: sizeof(x) + sizeof(x))

void test_sizeof_double(void) {
    int i = 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:52: warning: generic association expression uses parameter 'i' 2 times; parameters should be used at most once [automotive-generic-parameter-usage]
    unsigned long result = SIZE_DOUBLE(i);
    (void)result;
}

// Non-compliant: parameter used multiple times in more complex expression
#define COMPLEX_MULTI(x) _Generic((x), \
    int: (x) * (x) + (x), \
    float: (x))

void test_complex_multiple(void) {
    int i = 3;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: generic association expression uses parameter 'i' 3 times; parameters should be used at most once [automotive-generic-parameter-usage]
    int result = COMPLEX_MULTI(i);
    (void)result;
}

// Non-compliant: both controlling and association use parameter multiple times
#define MULTI_BOTH(x) _Generic(sizeof(x) + sizeof(x), \
    unsigned long: (x) + (x))

void test_multiple_in_both(void) {
    int i = 5;
    // This should trigger warnings for both controlling and association expressions
    // CHECK-MESSAGES: :[[@LINE+2]]:36: warning: generic selection controlling expression uses parameter 'i' 2 times; parameters should be used at most once [automotive-generic-parameter-usage]
    // CHECK-MESSAGES: :[[@LINE+1]]:36: warning: generic association expression uses parameter 'i' 2 times; parameters should be used at most once [automotive-generic-parameter-usage]
    int result = MULTI_BOTH(i);
    (void)result;
}

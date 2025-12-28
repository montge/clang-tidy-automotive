// XFAIL: *
// Test file for: automotive-generic-parameter-type-check
// A generic parameter should be type-checked in all generic associations
//
// RUN: %check_clang_tidy %s automotive-generic-parameter-type-check %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void process_int(int x) { (void)x; }
void process_float(float x) { (void)x; }

// Compliant: parameter x is used in all associations
#define PROCESS_ALL(x) _Generic((x), \
    int: process_int(x), \
    float: process_float(x))

void test_param_in_all_assocs(void) {
    int i = 10;
    float f = 5.5f;

    PROCESS_ALL(i);
    PROCESS_ALL(f);
}

// Compliant: consistent parameter usage
#define TYPE_SIZE(x) _Generic((x), \
    int: sizeof(x), \
    float: sizeof(x), \
    double: sizeof(x))

void test_consistent_usage(void) {
    int x = 5;
    unsigned long result = TYPE_SIZE(x);
    (void)result;
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

// Non-compliant: x not used in float association
#define INCONSISTENT1(x) _Generic((x), \
    int: process_int(x), \
    float: 0)

void test_missing_param(void) {
    int i = 10;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: generic parameter 'i' should be type-checked in all associations but is missing in this one [automotive-generic-parameter-type-check]
    INCONSISTENT1(i);
}

// Non-compliant: parameter used in some but not all associations
#define PARTIAL_USE(x) _Generic((x), \
    int: (x) * 2, \
    float: 100, \
    double: (x) * 3)

void test_partial_use(void) {
    float f = 5.0f;
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: generic parameter 'f' should be type-checked in all associations but is missing in this one [automotive-generic-parameter-type-check]
    PARTIAL_USE(f);
}

// Non-compliant: different parameters used in different associations
void use_int(int x) { (void)x; }
void use_float(float y) { (void)y; }

#define MIXED_PARAMS(x, y) _Generic((x), \
    int: use_int(x), \
    float: use_float(y))

void test_mixed_params(void) {
    int i = 1;
    float f = 2.0f;
    // This is testing if different parameters are detected
    MIXED_PARAMS(i, f);
}

// RUN: %check_clang_tidy %s automotive-cpp23-req-6.5 %t

// Test: Floating-point loop counter (MISRA C++:2023 Rule 6.5)
// Check ID: automotive-cpp23-req-6.5

// ============= Compliant: Integer loop counters =============

void test_compliant_int() {
    for (int i = 0; i < 10; i++) {
        // OK - integer loop counter
    }
}

void test_compliant_unsigned() {
    for (unsigned u = 0; u < 10; u++) {
        // OK - unsigned integer loop counter
    }
}

void test_compliant_long() {
    for (long l = 0; l < 100; l++) {
        // OK - long integer loop counter
    }
}

// ============= Violations: Float loop counter =============

void test_float_counter() {
    // CHECK-MESSAGES: :[[@LINE+2]]:16: warning: floating-point type 'float' used as loop counter
    // CHECK-MESSAGES: :[[@LINE+1]]:37: warning: floating-point type 'float' used as loop counter
    for (float f = 0.0f; f < 10.0f; f += 0.1f) {
    }
}

void test_float_decl_only() {
    // CHECK-MESSAGES: :[[@LINE+2]]:16: warning: floating-point type 'float' used as loop counter
    // CHECK-MESSAGES: :[[@LINE+1]]:36: warning: floating-point type 'float' used as loop counter
    for (float f = 0.0f; f < 1.0f; f++) {
    }
}

// ============= Violations: Double loop counter =============

void test_double_counter() {
    // CHECK-MESSAGES: :[[@LINE+2]]:17: warning: floating-point type 'double' used as loop counter
    // CHECK-MESSAGES: :[[@LINE+1]]:35: warning: floating-point type 'double' used as loop counter
    for (double d = 0.0; d < 1.0; d += 0.01) {
    }
}

void test_double_decrement() {
    // CHECK-MESSAGES: :[[@LINE+2]]:17: warning: floating-point type 'double' used as loop counter
    // CHECK-MESSAGES: :[[@LINE+1]]:36: warning: floating-point type 'double' used as loop counter
    for (double d = 10.0; d > 0.0; d--) {
    }
}

// ============= Violations: Long double loop counter =============

void test_long_double_counter() {
    // CHECK-MESSAGES: :[[@LINE+2]]:22: warning: floating-point type 'long double' used as loop counter
    // CHECK-MESSAGES: :[[@LINE+1]]:44: warning: floating-point type 'long double' used as loop counter
    for (long double ld = 0.0L; ld < 1.0L; ld += 0.1L) {
    }
}

// ============= Violations: External float variable as counter =============

void test_external_float_counter() {
    float f;
    // CHECK-MESSAGES: :[[@LINE+2]]:10: warning: floating-point type 'float' used as loop counter
    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: floating-point type 'float' used as loop counter
    for (f = 0.0f; f < 10.0f; f += 1.0f) {
    }
}

void test_external_double_counter() {
    double d;
    // CHECK-MESSAGES: :[[@LINE+2]]:10: warning: floating-point type 'double' used as loop counter
    // CHECK-MESSAGES: :[[@LINE+1]]:29: warning: floating-point type 'double' used as loop counter
    for (d = 0.0; d < 10.0; d++) {
    }
}

// ============= Compliant: Float used in body but not as counter =============

void test_float_in_body() {
    float sum = 0.0f;
    for (int i = 0; i < 10; i++) {
        sum += 0.1f;  // OK - float used in body, not as counter
    }
}

// ============= Compliant: Float in condition but integer counter =============

void test_float_condition() {
    float limit = 10.0f;
    for (int i = 0; i < limit; i++) {
        // OK - float in condition, but counter is integer
    }
}

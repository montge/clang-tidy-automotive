// RUN: %check_clang_tidy %s automotive-c25-req-12.6 %t -- -- -Wno-atomic-access

// Test: Atomic struct/union member access (MISRA C:2025 Rule 12.6)
// Check ID: automotive-c25-req-12.6
//
// Note: Clang already issues errors for atomic struct/union member access.
// This check provides an additional MISRA-specific warning message.

// ============= Compliant: Non-atomic struct access =============

struct Point {
    int x;
    int y;
};

struct Point regular_point;

void compliant_access() {
    regular_point.x = 10;  // OK - not atomic
    int val = regular_point.y;  // OK - not atomic
}

// ============= Violations: Atomic struct member access =============

_Atomic struct Point atomic_point;

void non_compliant_access() {
    // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: direct member access on atomic struct/union breaks atomicity
    int x = atomic_point.x;

    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: direct member access on atomic struct/union breaks atomicity
    atomic_point.y = 20;
}

// ============= Atomic union member access =============

union Data {
    int i;
    float f;
};

_Atomic union Data atomic_data;

void non_compliant_union_access() {
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: direct member access on atomic struct/union breaks atomicity
    int i = atomic_data.i;

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: direct member access on atomic struct/union breaks atomicity
    atomic_data.f = 3.14f;
}

// ============= Pointer to atomic struct =============

_Atomic struct Point *atomic_ptr;

void pointer_access() {
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: direct member access on atomic struct/union breaks atomicity
    int x = atomic_ptr->x;

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: direct member access on atomic struct/union breaks atomicity
    atomic_ptr->y = 30;
}

// ============= Edge cases: atomic simple types are fine =============

_Atomic int atomic_int;

void atomic_simple_type() {
    atomic_int = 42;  // OK - not struct/union member access
    int val = atomic_int;  // OK
}

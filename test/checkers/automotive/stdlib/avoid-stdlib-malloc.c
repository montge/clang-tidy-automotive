// Test file for: automotive-c23-req-21.3
// Related MISRA C:2025 Rule: 21.3, Directive 4.12
//
// This file tests the detection of dynamic memory allocation functions

// RUN: clang-tidy %s --checks='-*,automotive-c23-req-21.3' -- 2>&1 | FileCheck %s

#include <stdlib.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_malloc_violations(void) {
    // CHECK-DAG: warning: use of 'malloc' is not allowed in safety-critical code
    void *p1 = malloc(100);

    // CHECK-DAG: warning: use of 'calloc' is not allowed in safety-critical code
    void *p2 = calloc(10, sizeof(int));

    // CHECK-DAG: warning: use of 'realloc' is not allowed in safety-critical code
    p1 = realloc(p1, 200);

    // CHECK-DAG: warning: use of 'free' is not allowed in safety-critical code
    free(p1);
    // CHECK-DAG: warning: use of 'free' is not allowed in safety-critical code
    free(p2);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

// Static allocation
static int buffer[100];

void test_compliant(void) {
    // Stack allocation
    int local_array[50];

    // Using static buffer
    buffer[0] = 1;
    local_array[0] = 2;
}

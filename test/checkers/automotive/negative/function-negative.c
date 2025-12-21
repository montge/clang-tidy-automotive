// Negative test file for: automotive function checks
// Related MISRA C:2025 Rules: 8.2, 17.7, 17.8, 15.5
//
// This file contains code that should NOT trigger any warnings.
// All code here is compliant with MISRA rules.

// RUN: clang-tidy -checks='-*,automotive-uncomplete-function-prototype,automotive-missing-return-value-handling,automotive-avoid-function-parameter-modification,automotive-avoid-multiple-return-stmt' %s -- 2>&1 | FileCheck %s -allow-empty -check-prefix=CHECK-NEGATIVE
// CHECK-NEGATIVE-NOT: warning:

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//===----------------------------------------------------------------------===//
// Negative: Complete function prototypes
//===----------------------------------------------------------------------===//

// Function with void parameter
int get_value(void);

// Function with named parameters
int add(int a, int b);
void process_data(int *data, size_t size);

// Function pointer with complete prototype
typedef int (*operation_fn)(int a, int b);
typedef void (*callback_fn)(void *context);

// Declarations match definitions
int get_value(void) {
    return 42;
}

int add(int a, int b) {
    return a + b;
}

void process_data(int *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        data[i] = 0;
    }
}

//===----------------------------------------------------------------------===//
// Negative: Return values handled
//===----------------------------------------------------------------------===//

int compute(int x) { return x * x; }
void *allocate_memory(size_t size) { return malloc(size); }

void test_return_handled(void) {
    // Return value assigned to variable
    int result = compute(5);
    (void)result;

    // Return value used in expression
    int doubled = compute(3) * 2;
    (void)doubled;

    // Return value used in condition
    if (compute(2) > 0) {
        // do something
    }

    // Pointer return value assigned
    void *ptr = allocate_memory(100);
    free(ptr);

    // Return value explicitly ignored with cast
    (void)compute(1);
}

//===----------------------------------------------------------------------===//
// Negative: Parameters not modified
//===----------------------------------------------------------------------===//

int double_value(int x) {
    // Parameter used but not modified
    return x * 2;
}

void copy_array(const int *src, int *dst, size_t count) {
    // src is const - cannot be modified
    // dst points to data that is modified, but dst itself isn't
    for (size_t i = 0; i < count; i++) {
        dst[i] = src[i];
    }
}

int sum_array(const int *arr, size_t size) {
    int total = 0;
    for (size_t i = 0; i < size; i++) {
        total += arr[i];  // arr not modified
    }
    return total;
}

void use_local_copy(int x) {
    int local = x;  // Copy to local
    local++;        // Modify local, not parameter
    (void)local;
}

//===----------------------------------------------------------------------===//
// Negative: Single return statement
//===----------------------------------------------------------------------===//

int single_return_simple(int x) {
    int result = x * 2;
    return result;
}

int single_return_conditional(int x) {
    int result;
    if (x > 0) {
        result = 1;
    } else if (x < 0) {
        result = -1;
    } else {
        result = 0;
    }
    return result;
}

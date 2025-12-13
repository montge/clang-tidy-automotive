// Negative test file for: automotive function checks
// Related MISRA C:2025 Rules: 8.2, 17.3, 17.7, 17.8, 15.5
//
// This file contains code that should NOT trigger any warnings.
// All code here is compliant with MISRA rules.

// RUN: %check_clang_tidy %s automotive-uncomplete-function-prototype,automotive-implicit-function-decl,automotive-missing-return-value-handling,automotive-avoid-function-parameter-modification,automotive-avoid-multiple-return-stmt %t
// CHECK-MESSAGES-NOT: warning:

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
void process(int *data, size_t size);

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

void process(int *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        data[i] = 0;
    }
}

//===----------------------------------------------------------------------===//
// Negative: No implicit function declarations
//===----------------------------------------------------------------------===//

// All functions declared before use
int helper_function(int x);

void test_no_implicit(void) {
    int result = helper_function(10);  // Function declared above
    (void)result;
}

int helper_function(int x) {
    return x * 2;
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

void test_void_functions(void) {
    // void functions have no return value to handle
    process(NULL, 0);  // OK - void function
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

int single_return_switch(int cmd) {
    int result;
    switch (cmd) {
        case 1:
            result = 10;
            break;
        case 2:
            result = 20;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

//===----------------------------------------------------------------------===//
// Negative: Complex compliant functions
//===----------------------------------------------------------------------===//

bool validate_and_process(const int *data, size_t size, int *output) {
    // Single return, complete prototype, parameters not modified
    bool success;

    if (data == NULL || output == NULL || size == 0) {
        success = false;
    } else {
        int sum = 0;
        for (size_t i = 0; i < size; i++) {
            sum += data[i];
        }
        *output = sum;
        success = true;
    }

    return success;
}

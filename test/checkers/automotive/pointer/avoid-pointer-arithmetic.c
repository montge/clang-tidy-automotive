// RUN: %check_clang_tidy %s automotive-c25-adv-18.4 %t

// Test: Pointer arithmetic with +, -, +=, -= operators (MISRA C:2025 Rule 18.4)
// Check ID: automotive-c25-adv-18.4

#include <stddef.h>

//===----------------------------------------------------------------------===//
// Violation Cases: Binary + operator with pointers
//===----------------------------------------------------------------------===//

void test_pointer_addition_violations(void) {
    int arr[10];
    int *ptr = arr;

    // Pointer + integer
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer arithmetic using '+' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr = ptr + 1;

    // Integer + pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer arithmetic using '+' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr = 2 + ptr;

    // Pointer + offset
    int offset = 3;
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: pointer arithmetic using '+' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr = ptr + offset;

    // Using result directly
    // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: pointer arithmetic using '+' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    int x = *(ptr + 1);

    (void)x;
}

//===----------------------------------------------------------------------===//
// Violation Cases: Binary - operator with pointers
//===----------------------------------------------------------------------===//

void test_pointer_subtraction_violations(void) {
    int arr[10];
    int *ptr1 = &arr[5];
    int *ptr2 = &arr[2];

    // Pointer - integer
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: pointer arithmetic using '-' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr1 = ptr1 - 1;

    // Pointer - offset
    int offset = 2;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: pointer arithmetic using '-' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr1 = ptr1 - offset;

    // Pointer difference (ptr1 - ptr2)
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: pointer difference using '-' operator on pointer types 'int *' and 'int *' [automotive-c25-adv-18.4]
    ptrdiff_t diff = ptr1 - ptr2;

    // Using result directly
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: pointer arithmetic using '-' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    int y = *(ptr1 - 1);

    (void)diff; (void)y;
}

//===----------------------------------------------------------------------===//
// Violation Cases: Compound assignment operators += and -=
//===----------------------------------------------------------------------===//

void test_compound_assignment_violations(void) {
    int arr[10];
    int *ptr = arr;

    // Compound assignment +=
    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: compound assignment '+=' operator used on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr += 1;

    // Compound assignment += with variable
    int offset = 2;
    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: compound assignment '+=' operator used on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr += offset;

    // Compound assignment -=
    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: compound assignment '-=' operator used on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr -= 1;

    // Compound assignment -= with variable
    // CHECK-MESSAGES: :[[@LINE+1]]:8: warning: compound assignment '-=' operator used on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr -= offset;
}

//===----------------------------------------------------------------------===//
// Violation Cases: Various pointer types
//===----------------------------------------------------------------------===//

void test_various_pointer_types(void) {
    char str[20];
    char *cptr = str;

    // Character pointer arithmetic
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: pointer arithmetic using '+' operator on pointer type 'char *'; use array subscript notation instead [automotive-c25-adv-18.4]
    cptr = cptr + 1;

    double darr[5];
    double *dptr = darr;

    // Double pointer arithmetic
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: pointer arithmetic using '+' operator on pointer type 'double *'; use array subscript notation instead [automotive-c25-adv-18.4]
    dptr = dptr + 2;

    void *vptr = str;
    // Void pointer arithmetic (implementation-defined but still checked)
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: pointer arithmetic using '+' operator on pointer type 'void *'; use array subscript notation instead [automotive-c25-adv-18.4]
    vptr = vptr + 1;
}

//===----------------------------------------------------------------------===//
// Compliant Cases: Array subscript notation
//===----------------------------------------------------------------------===//

void test_compliant_array_subscript(void) {
    int arr[10];
    int *ptr = arr;

    // Array subscript notation (compliant)
    int value1 = arr[0];
    int value2 = arr[5];
    int value3 = ptr[3];

    // Assignment using array indexing
    arr[0] = 42;
    ptr[1] = 43;

    (void)value1; (void)value2; (void)value3;
}

//===----------------------------------------------------------------------===//
// Compliant Cases: Pointer assignment and dereferencing
//===----------------------------------------------------------------------===//

void test_compliant_pointer_operations(void) {
    int x = 42;
    int y = 43;
    int *ptr1 = &x;
    int *ptr2 = &y;

    // Pointer assignment (no arithmetic)
    ptr1 = ptr2;

    // Dereferencing
    int value = *ptr1;

    // Address-of
    int *ptr3 = &x;

    // Null pointer
    int *ptr4 = (void *)0;

    (void)value; (void)ptr3; (void)ptr4;
}

//===----------------------------------------------------------------------===//
// Compliant Cases: Increment and decrement operators
//===----------------------------------------------------------------------===//

void test_increment_decrement(void) {
    int arr[10];
    int *ptr = arr;

    // Note: ++ and -- are not covered by Rule 18.4
    // They are covered by Rule 18.3 (not implemented in this check)
    // These should NOT generate warnings from this check
    ptr++;
    ++ptr;
    ptr--;
    --ptr;
}

//===----------------------------------------------------------------------===//
// Compliant Cases: Pointer comparison
//===----------------------------------------------------------------------===//

void test_pointer_comparison(void) {
    int arr[10];
    int *ptr1 = &arr[0];
    int *ptr2 = &arr[5];

    // Pointer comparison (no arithmetic)
    if (ptr1 < ptr2) {
        // Do something
    }

    if (ptr1 == ptr2) {
        // Do something
    }

    if (ptr1 != ptr2) {
        // Do something
    }
}

//===----------------------------------------------------------------------===//
// Edge Cases: Complex expressions
//===----------------------------------------------------------------------===//

void test_complex_expressions(void) {
    int arr[10];
    int *ptr = arr;

    // Multiple arithmetic operations
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: pointer arithmetic using '+' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    int value1 = *((ptr + 1));

    // Nested operations
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: pointer arithmetic using '+' operator on pointer type 'int *'; use array subscript notation instead [automotive-c25-adv-18.4]
    ptr = (ptr + 2);

    (void)value1;
}

//===----------------------------------------------------------------------===//
// Compliant Cases: Using array base directly
//===----------------------------------------------------------------------===//

void test_array_base_operations(void) {
    int arr[10];

    // Direct array indexing (compliant)
    int value1 = arr[0];
    int value2 = arr[5];

    // Taking address of array element (compliant)
    int *ptr1 = &arr[3];
    int *ptr2 = &arr[7];

    (void)value1; (void)value2; (void)ptr1; (void)ptr2;
}

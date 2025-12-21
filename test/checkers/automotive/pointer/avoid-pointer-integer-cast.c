// RUN: %check_clang_tidy %s automotive-c23-adv-11.4 %t -- -- -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

// Test: Casts between pointers and integers (MISRA Rules 11.4, 11.6)
// Check ID: automotive-c23-adv-11.4

#include <stdint.h>
#include <stddef.h>

//===----------------------------------------------------------------------===//
// Violation Cases: Pointer to Integer
//===----------------------------------------------------------------------===//

void test_pointer_to_integer_violations(void) {
    int x = 42;
    int *ptr = &x;

    // Cast pointer to long
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: cast from pointer type 'int *' to integer type 'long' [automotive-c23-adv-11.4]
    long value1 = (long)ptr;

    // Cast pointer to unsigned long
    // CHECK-MESSAGES: :[[@LINE+1]]:28: warning: cast from pointer type 'int *' to integer type 'unsigned long' [automotive-c23-adv-11.4]
    unsigned long value2 = (unsigned long)ptr;

    // Cast pointer to intptr_t
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: cast from pointer type 'int *' to integer type 'intptr_t' {{.*}} [automotive-c23-adv-11.4]
    intptr_t value3 = (intptr_t)ptr;

    // Cast pointer to uintptr_t
    // CHECK-MESSAGES: :[[@LINE+1]]:24: warning: cast from pointer type 'int *' to integer type 'uintptr_t' {{.*}} [automotive-c23-adv-11.4]
    uintptr_t value4 = (uintptr_t)ptr;

    // Cast pointer to size_t
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: cast from pointer type 'int *' to integer type 'size_t' {{.*}} [automotive-c23-adv-11.4]
    size_t value5 = (size_t)ptr;

    (void)value1; (void)value2; (void)value3; (void)value4; (void)value5;
}

//===----------------------------------------------------------------------===//
// Violation Cases: Integer to Pointer
//===----------------------------------------------------------------------===//

void test_integer_to_pointer_violations(void) {
    int *int_ptr;
    void *void_ptr;

    long lvalue = 0x1000;

    // Cast long to pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from integer type 'long' to pointer type 'int *' [automotive-c23-adv-11.4]
    int_ptr = (int *)lvalue;

    // Cast intptr_t to pointer
    intptr_t iptr = 0x2000;
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from integer type 'intptr_t' {{.*}} to pointer type 'int *' [automotive-c23-adv-11.4]
    int_ptr = (int *)iptr;

    // Cast uintptr_t to pointer
    uintptr_t uiptr = 0x3000;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from integer type 'uintptr_t' {{.*}} to pointer type 'void *' [automotive-c23-adv-11.4]
    void_ptr = (void *)uiptr;

    (void)int_ptr; (void)void_ptr;
}

//===----------------------------------------------------------------------===//
// Violation Cases: Round-trip casts
//===----------------------------------------------------------------------===//

void test_round_trip_violations(void) {
    int x = 42;
    int *original_ptr = &x;
    int *restored_ptr;

    // Pointer to integer
    // CHECK-MESSAGES: :[[@LINE+1]]:26: warning: cast from pointer type 'int *' to integer type 'unsigned long' [automotive-c23-adv-11.4]
    unsigned long addr = (unsigned long)original_ptr;

    // Integer back to pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: cast from integer type 'unsigned long' to pointer type 'int *' [automotive-c23-adv-11.4]
    restored_ptr = (int *)addr;

    (void)restored_ptr;
}

//===----------------------------------------------------------------------===//
// Compliant Cases
//===----------------------------------------------------------------------===//

void test_null_pointer_constant(void) {
    int *ptr1;
    void *ptr2;

    // NULL pointer constant (0) - allowed
    ptr1 = (int *)0;
    ptr2 = (void *)0;

    (void)ptr1; (void)ptr2;
}

void test_void_pointer_casts(void) {
    int x = 42;
    int *int_ptr = &x;
    void *void_ptr;

    // Cast to/from void* (pointer to pointer, not integer)
    void_ptr = (void *)int_ptr;
    int_ptr = (int *)void_ptr;

    (void)int_ptr; (void)void_ptr;
}

void test_normal_pointer_operations(void) {
    int x = 42;
    int *ptr = &x;

    // Normal pointer assignment
    int *ptr2 = ptr;

    // Dereference
    int value = *ptr;

    // Address-of
    int *ptr3 = &x;

    (void)ptr2; (void)value; (void)ptr3;
}

void test_pointer_arithmetic(void) {
    int arr[10];
    int *ptr = arr;

    // Pointer arithmetic (no cast to/from integer)
    ptr = ptr + 1;
    ptr = ptr - 1;
    ptr++;
    --ptr;

    // Pointer difference (result is ptrdiff_t)
    ptrdiff_t diff = &arr[5] - &arr[2];
    (void)diff;
}

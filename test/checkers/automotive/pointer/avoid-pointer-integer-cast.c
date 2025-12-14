// Test file for: automotive-avoid-pointer-integer-cast
// Related MISRA C:2025 Rule: 11.4, 11.6
//
// This file tests the detection of casts between pointers and integers

// RUN: %check_clang_tidy %s automotive-avoid-pointer-integer-cast %t

#include <stdint.h>
#include <stddef.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_pointer_to_integer_violations(void) {
    int x = 42;
    int *ptr = &x;
    float *fptr;
    char *cptr = "test";

    // Cast pointer to int
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from pointer type 'int *' to integer type 'int' [automotive-avoid-pointer-integer-cast]
    int value1 = (int)ptr;

    // Cast pointer to unsigned int
    // CHECK-MESSAGES: :[[@LINE+1]]:25: warning: cast from pointer type 'int *' to integer type 'unsigned int' [automotive-avoid-pointer-integer-cast]
    unsigned int value2 = (unsigned int)ptr;

    // Cast pointer to long
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: cast from pointer type 'int *' to integer type 'long' [automotive-avoid-pointer-integer-cast]
    long value3 = (long)ptr;

    // Cast pointer to unsigned long
    // CHECK-MESSAGES: :[[@LINE+1]]:27: warning: cast from pointer type 'int *' to integer type 'unsigned long' [automotive-avoid-pointer-integer-cast]
    unsigned long value4 = (unsigned long)ptr;

    // Cast pointer to short
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: cast from pointer type 'float *' to integer type 'short' [automotive-avoid-pointer-integer-cast]
    short value5 = (short)fptr;

    // Cast pointer to char (as integer type)
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: cast from pointer type 'char *' to integer type 'char' [automotive-avoid-pointer-integer-cast]
    char value6 = (char)cptr;

    // Cast pointer to intptr_t (even though designed for this, it's still a cast)
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: cast from pointer type 'int *' to integer type 'intptr_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    intptr_t value7 = (intptr_t)ptr;

    // Cast pointer to uintptr_t
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: cast from pointer type 'int *' to integer type 'uintptr_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    uintptr_t value8 = (uintptr_t)ptr;

    // Cast pointer to ptrdiff_t
    // CHECK-MESSAGES: :[[@LINE+1]]:23: warning: cast from pointer type 'int *' to integer type 'ptrdiff_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    ptrdiff_t value9 = (ptrdiff_t)ptr;

    // Cast pointer to size_t
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: cast from pointer type 'int *' to integer type 'size_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    size_t value10 = (size_t)ptr;
}

void test_integer_to_pointer_violations(void) {
    int *int_ptr;
    float *float_ptr;
    char *char_ptr;
    void *void_ptr;

    int value = 0x1234;
    unsigned int uvalue = 0x5678;
    long lvalue = 0xABCD;

    // Cast int to pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from integer type 'int' to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    int_ptr = (int *)value;

    // Cast unsigned int to pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: cast from integer type 'unsigned int' to pointer type 'float *' [automotive-avoid-pointer-integer-cast]
    float_ptr = (float *)uvalue;

    // Cast long to pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from integer type 'long' to pointer type 'char *' [automotive-avoid-pointer-integer-cast]
    char_ptr = (char *)lvalue;

    // Cast intptr_t to pointer
    intptr_t iptr = 0x1000;
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from integer type 'intptr_t' (aka {{.*}}) to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    int_ptr = (int *)iptr;

    // Cast uintptr_t to pointer
    uintptr_t uiptr = 0x2000;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from integer type 'uintptr_t' (aka {{.*}}) to pointer type 'void *' [automotive-avoid-pointer-integer-cast]
    void_ptr = (void *)uiptr;

    // Cast from hex literal
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from integer type 'int' to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    int_ptr = (int *)0x12345678;

    // Cast size_t to pointer
    size_t sval = 0x3000;
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from integer type 'size_t' (aka {{.*}}) to pointer type 'void *' [automotive-avoid-pointer-integer-cast]
    void_ptr = (void *)sval;
}

void test_round_trip_violations(void) {
    int x = 42;
    int *original_ptr = &x;
    int *restored_ptr;

    // Round-trip: pointer -> integer -> pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: cast from pointer type 'int *' to integer type 'unsigned long' [automotive-avoid-pointer-integer-cast]
    unsigned long addr = (unsigned long)original_ptr;

    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: cast from integer type 'unsigned long' to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    restored_ptr = (int *)addr;
}

void test_pointer_arithmetic_via_integer(void) {
    int arr[10];
    int *ptr = arr;

    // Calculate offset via integer cast
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: cast from pointer type 'int *' to integer type 'uintptr_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    uintptr_t addr1 = (uintptr_t)ptr;
    uintptr_t addr2 = addr1 + sizeof(int) * 5;
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: cast from integer type 'uintptr_t' (aka {{.*}}) to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    int *ptr2 = (int *)addr2;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_null_pointer_constant(void) {
    int *ptr1;
    float *ptr2;
    void *ptr3;

    // NULL pointer constant (0) - allowed
    ptr1 = (int *)0;
    ptr2 = (float *)0;
    ptr3 = (void *)0;

    // Using macro NULL
    ptr1 = ((void *)0);  // NULL is typically defined as this
}

void test_normal_pointer_operations(void) {
    int x = 42;
    int *ptr1 = &x;
    int *ptr2;

    // Normal pointer assignment
    ptr2 = ptr1;

    // Dereference
    int value = *ptr1;

    // Address-of
    int *ptr3 = &x;

    // Array indexing
    int arr[10];
    ptr1 = &arr[5];
}

void test_pointer_arithmetic(void) {
    int arr[10];
    int *ptr = arr;

    // Pointer arithmetic (no cast to/from integer)
    ptr = ptr + 1;
    ptr = ptr - 1;
    ptr++;
    ptr--;
    ++ptr;
    --ptr;

    // Pointer difference (result is ptrdiff_t, not a cast)
    ptrdiff_t diff = &arr[5] - &arr[2];
}

void test_pointer_comparison(void) {
    int arr[10];
    int *ptr1 = &arr[0];
    int *ptr2 = &arr[5];

    // Pointer comparisons don't involve casts
    if (ptr1 < ptr2) {}
    if (ptr1 == ptr2) {}
    if (ptr1 != ptr2) {}
}

void test_void_pointer_casts(void) {
    int x = 42;
    int *int_ptr = &x;
    void *void_ptr;

    // Cast to/from void* (pointer to pointer, not integer)
    void_ptr = (void *)int_ptr;
    int_ptr = (int *)void_ptr;
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_function_pointers(void) {
    typedef void (*FuncPtr)(void);

    void (*func)(void);
    FuncPtr fptr;

    // Cast function pointer to integer
    // CHECK-MESSAGES: :[[@LINE+1]>::22: warning: cast from pointer type 'void (*)(void)' to integer type 'uintptr_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    uintptr_t addr = (uintptr_t)func;

    // Cast integer to function pointer
    uintptr_t faddr = 0x1000;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: cast from integer type 'uintptr_t' (aka {{.*}}) to pointer type 'FuncPtr' (aka 'void (*)(void)') [automotive-avoid-pointer-integer-cast]
    fptr = (FuncPtr)faddr;
}

void test_hardware_register_access(void) {
    // Common pattern in embedded systems (but still violates MISRA)
    volatile unsigned int *reg_ptr;

    // Memory-mapped I/O address
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from integer type 'int' to pointer type 'volatile unsigned int *' [automotive-avoid-pointer-integer-cast]
    reg_ptr = (volatile unsigned int *)0x40000000;

    // Write to register
    *reg_ptr = 0x12345678;

    // Read from register
    unsigned int reg_value = *reg_ptr;
}

void test_alignment_check(void) {
    int x;
    int *ptr = &x;

    // Check alignment by converting to integer
    // CHECK-MESSAGES: :[[@LINE+1]>::22: warning: cast from pointer type 'int *' to integer type 'uintptr_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    uintptr_t addr = (uintptr_t)ptr;
    int is_aligned = (addr % sizeof(int)) == 0;
}

void test_pointer_masking(void) {
    void *ptr;

    // Mask lower bits of pointer (common in memory allocators)
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: cast from pointer type 'void *' to integer type 'uintptr_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t masked = addr & ~0xFUL;  // Align to 16 bytes
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: cast from integer type 'uintptr_t' (aka {{.*}}) to pointer type 'void *' [automotive-avoid-pointer-integer-cast]
    void *aligned = (void *)masked;
}

void test_stdint_types(void) {
    int *ptr;

    // int8_t to pointer
    int8_t i8 = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: cast from integer type 'int8_t' (aka {{.*}}) to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    ptr = (int *)i8;

    // int16_t to pointer
    int16_t i16 = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: cast from integer type 'int16_t' (aka {{.*}}) to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    ptr = (int *)i16;

    // int32_t to pointer
    int32_t i32 = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: cast from integer type 'int32_t' (aka {{.*}}) to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    ptr = (int *)i32;

    // int64_t to pointer
    int64_t i64 = 0;
    // CHECK-MESSAGES: :[[@LINE+1]]:11: warning: cast from integer type 'int64_t' (aka {{.*}}) to pointer type 'int *' [automotive-avoid-pointer-integer-cast]
    ptr = (int *)i64;
}

void test_pointer_tagging(void) {
    // Pointer tagging: storing flags in unused pointer bits
    void *ptr;

    // Extract address bits
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: cast from pointer type 'void *' to integer type 'uintptr_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t tagged = addr | 0x1;  // Set tag bit
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from integer type 'uintptr_t' (aka {{.*}}) to pointer type 'void *' [automotive-avoid-pointer-integer-cast]
    void *tagged_ptr = (void *)tagged;

    // Extract tag
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: cast from pointer type 'void *' to integer type 'uintptr_t' (aka {{.*}}) [automotive-avoid-pointer-integer-cast]
    uintptr_t bits = (uintptr_t)tagged_ptr;
    int tag = bits & 0x1;
}

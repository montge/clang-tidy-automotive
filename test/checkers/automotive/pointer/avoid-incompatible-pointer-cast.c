// Test file for: automotive-c23-req-11.3
// Related MISRA C:2025 Rule: 11.3
//
// This file tests the detection of casts between incompatible pointer types

// RUN: %check_clang_tidy %s automotive-c23-req-11.3 %t

#include <stdint.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_incompatible_pointer_casts(void) {
    int *int_ptr;
    float *float_ptr;
    double *double_ptr;
    short *short_ptr;
    long *long_ptr;

    // Cast between different numeric types
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: cast from 'int *' to 'float *' converts between incompatible pointer types [automotive-c23-req-11.3]
    float_ptr = (float *)int_ptr;

    // Cast from int* to double*
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: cast from 'int *' to 'double *' converts between incompatible pointer types [automotive-c23-req-11.3]
    double_ptr = (double *)int_ptr;

    // Cast from short* to long*
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from 'short *' to 'long *' converts between incompatible pointer types [automotive-c23-req-11.3]
    long_ptr = (long *)short_ptr;

    // Cast from float* to int*
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from 'float *' to 'int *' converts between incompatible pointer types [automotive-c23-req-11.3]
    int_ptr = (int *)float_ptr;

    // Cast from double* to short*
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: cast from 'double *' to 'short *' converts between incompatible pointer types [automotive-c23-req-11.3]
    short_ptr = (short *)double_ptr;
}

void test_struct_pointer_casts(void) {
    struct TypeA {
        int x;
        int y;
    };

    struct TypeB {
        float a;
        float b;
    };

    struct TypeA *ptr_a;
    struct TypeB *ptr_b;

    // Cast between different struct types
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: cast from 'struct TypeB *' to 'struct TypeA *' converts between incompatible pointer types [automotive-c23-req-11.3]
    ptr_a = (struct TypeA *)ptr_b;

    // Reverse cast
    // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: cast from 'struct TypeA *' to 'struct TypeB *' converts between incompatible pointer types [automotive-c23-req-11.3]
    ptr_b = (struct TypeB *)ptr_a;
}

void test_function_pointer_casts(void) {
    typedef void (*VoidFuncPtr)(void);
    typedef int (*IntFuncPtr)(int);
    typedef float (*FloatFuncPtr)(float);

    VoidFuncPtr void_func;
    IntFuncPtr int_func;
    FloatFuncPtr float_func;

    // Cast between different function pointer types
    // Note: Function pointer casts are not currently detected by this check
    int_func = (IntFuncPtr)void_func;

    // Cast from int function to float function
    // Note: Function pointer casts are not currently detected by this check
    float_func = (FloatFuncPtr)int_func;
}

void test_pointer_to_different_signedness(void) {
    int *int_ptr;
    unsigned int *uint_ptr;

    // Cast from signed to unsigned pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from 'int *' to 'unsigned int *' converts between incompatible pointer types [automotive-c23-req-11.3]
    uint_ptr = (unsigned int *)int_ptr;

    // Cast from unsigned to signed pointer
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from 'unsigned int *' to 'int *' converts between incompatible pointer types [automotive-c23-req-11.3]
    int_ptr = (int *)uint_ptr;
}

void test_array_pointer_casts(void) {
    int (*arr_ptr1)[10];
    int (*arr_ptr2)[20];

    // Cast between pointers to arrays of different sizes
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from 'int (*)[10]' to 'int (*)[20]' converts between incompatible pointer types [automotive-c23-req-11.3]
    arr_ptr2 = (int (*)[20])arr_ptr1;
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_void_pointer(void) {
    int *int_ptr;
    float *float_ptr;
    void *void_ptr;

    // Cast to void* is allowed
    void_ptr = (void *)int_ptr;
    void_ptr = (void *)float_ptr;

    // Cast from void* is allowed
    int_ptr = (int *)void_ptr;
    float_ptr = (float *)void_ptr;
}

void test_compliant_char_pointer(void) {
    int *int_ptr;
    float *float_ptr;
    char *char_ptr;
    unsigned char *uchar_ptr;

    // Cast to char* or unsigned char* is allowed (byte access)
    char_ptr = (char *)int_ptr;
    uchar_ptr = (unsigned char *)float_ptr;

    // Cast from char* is allowed
    int_ptr = (int *)char_ptr;
    float_ptr = (float *)uchar_ptr;
}

void test_compliant_same_type(void) {
    int *int_ptr1;
    int *int_ptr2;

    // Cast to same type (no warning)
    int_ptr2 = (int *)int_ptr1;

    // With qualifiers
    const int *const_int_ptr;
    int_ptr1 = (int *)const_int_ptr;  // Removing const (qualifier cast)
}

void test_compliant_compatible_types(void) {
    // Implicit conversions (no explicit cast needed)
    int arr[10];
    int *ptr = arr;  // Array to pointer decay

    // Pointer to compatible type
    int *int_ptr = &arr[0];
}

void test_compliant_null_pointer(void) {
    int *int_ptr;
    float *float_ptr;

    // NULL pointer assignments
    int_ptr = (int *)0;
    float_ptr = (float *)0;

    // Using NULL macro
    int_ptr = ((void *)0);
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_nested_pointer_casts(void) {
    int **int_ptr_ptr;
    float **float_ptr_ptr;

    // Cast between pointer-to-pointer types
    // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: cast from 'int **' to 'float **' converts between incompatible pointer types [automotive-c23-req-11.3]
    float_ptr_ptr = (float **)int_ptr_ptr;
}

void test_const_volatile_casts(void) {
    int *int_ptr;
    const int *const_int_ptr;
    volatile int *volatile_int_ptr;

    // Adding qualifiers - same base type, should be OK
    const_int_ptr = (const int *)int_ptr;
    volatile_int_ptr = (volatile int *)int_ptr;

    // Removing qualifiers - same base type
    int_ptr = (int *)const_int_ptr;
    int_ptr = (int *)volatile_int_ptr;
}

void test_typedef_casts(void) {
    typedef int Int32;
    typedef float Float32;

    Int32 *int32_ptr;
    Float32 *float32_ptr;
    int *int_ptr;

    // Cast to typedef'd pointer of same underlying type - should be OK
    int32_ptr = (Int32 *)int_ptr;
    int_ptr = (int *)int32_ptr;

    // Cast to typedef'd pointer of different type - should warn
    // CHECK-MESSAGES: :[[@LINE+1]]:19: warning: cast from 'Int32 *' (aka 'int *') to 'Float32 *' (aka 'float *') converts between incompatible pointer types [automotive-c23-req-11.3]
    float32_ptr = (Float32 *)int32_ptr;
}

void test_union_pointer_casts(void) {
    union Data {
        int i;
        float f;
    };

    union Data *data_ptr;
    void *void_ptr;

    // Cast to void* is OK
    void_ptr = (void *)data_ptr;

    // Cast from void* is OK
    data_ptr = (union Data *)void_ptr;
}

void test_enum_pointer_casts(void) {
    enum Color {
        RED,
        GREEN,
        BLUE
    };

    enum Color *color_ptr;
    int *int_ptr;

    // Enum and int are typically compatible, but check warns anyway
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: cast from 'int *' to 'enum Color *' converts between incompatible pointer types [automotive-c23-req-11.3]
    color_ptr = (enum Color *)int_ptr;
}

void test_stdint_types(void) {
    int32_t *int32_ptr;
    uint32_t *uint32_ptr;
    int16_t *int16_ptr;

    // Cast between different stdint types
    // CHECK-MESSAGES: :[[@LINE+1]]:18: warning: cast from 'int32_t *' (aka 'int *') to 'uint32_t *' (aka 'unsigned int *') converts between incompatible pointer types [automotive-c23-req-11.3]
    uint32_ptr = (uint32_t *)int32_ptr;

    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: cast from 'int32_t *' (aka 'int *') to 'int16_t *' (aka 'short *') converts between incompatible pointer types [automotive-c23-req-11.3]
    int16_ptr = (int16_t *)int32_ptr;
}

void test_opaque_pointers(void) {
    // Opaque pointer types (common in APIs)
    typedef struct OpaqueType1 *Handle1;
    typedef struct OpaqueType2 *Handle2;

    Handle1 h1;
    Handle2 h2;

    // Cast between opaque handles
    // Note: C-style casts of typedef'd pointers are not currently detected
    h1 = (Handle1)h2;
}

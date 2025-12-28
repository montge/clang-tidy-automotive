// RUN: %check_clang_tidy %s automotive-c25-req-11.7 %t

// Test: Casts between pointers and floating-point types (MISRA C:2025 Rule 11.7)
// Check ID: automotive-c25-req-11.7

//===----------------------------------------------------------------------===//
// Violation Cases: Pointer to Floating-Point via reinterpret_cast
//===----------------------------------------------------------------------===//

void test_pointer_to_float_violations() {
    int x = 42;
    int *ptr = &x;
    void *vptr = &x;

    // Cast pointer to float via reference (reinterpret_cast)
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from pointer type 'int *' to floating-point type 'float' [automotive-c25-req-11.7]
    float f1 = reinterpret_cast<float&>(ptr);

    // Cast pointer to double via reference (reinterpret_cast)
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: cast from pointer type 'int *' to floating-point type 'double' [automotive-c25-req-11.7]
    double d1 = reinterpret_cast<double&>(ptr);

    // Cast void pointer to float via reference
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from pointer type 'void *' to floating-point type 'float' [automotive-c25-req-11.7]
    float f2 = reinterpret_cast<float&>(vptr);

    // Cast void pointer to double via reference
    // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: cast from pointer type 'void *' to floating-point type 'double' [automotive-c25-req-11.7]
    double d2 = reinterpret_cast<double&>(vptr);

    // Cast pointer to long double via reference
    // CHECK-MESSAGES: :[[@LINE+1]]:22: warning: cast from pointer type 'int *' to floating-point type 'long double' [automotive-c25-req-11.7]
    long double ld = reinterpret_cast<long double&>(ptr);

    (void)f1; (void)d1; (void)f2; (void)d2; (void)ld;
}

//===----------------------------------------------------------------------===//
// Violation Cases: Floating-Point to Pointer via reinterpret_cast
//===----------------------------------------------------------------------===//

void test_float_to_pointer_violations() {
    float f = 3.14f;
    double d = 2.718;
    long double ld = 1.414L;

    int *int_ptr;
    void *void_ptr;
    char *char_ptr;

    // Cast float to pointer via reference
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from floating-point type 'float' to pointer type 'int *' [automotive-c25-req-11.7]
    int_ptr = reinterpret_cast<int*&>(f);

    // Cast double to pointer via reference
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from floating-point type 'double' to pointer type 'void *' [automotive-c25-req-11.7]
    void_ptr = reinterpret_cast<void*&>(d);

    // Cast long double to pointer via reference
    // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: cast from floating-point type 'long double' to pointer type 'char *' [automotive-c25-req-11.7]
    char_ptr = reinterpret_cast<char*&>(ld);

    (void)int_ptr; (void)void_ptr; (void)char_ptr;
}

//===----------------------------------------------------------------------===//
// Violation Cases: Round-trip casts
//===----------------------------------------------------------------------===//

void test_round_trip_violations() {
    int x = 42;
    int *original_ptr = &x;
    int *restored_ptr;

    // Pointer to float via reference
    // CHECK-MESSAGES: :[[@LINE+1]]:15: warning: cast from pointer type 'int *' to floating-point type 'float' [automotive-c25-req-11.7]
    float f = reinterpret_cast<float&>(original_ptr);

    // Float back to pointer via reference
    // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: cast from floating-point type 'float' to pointer type 'int *' [automotive-c25-req-11.7]
    restored_ptr = reinterpret_cast<int*&>(f);

    (void)restored_ptr;
}

//===----------------------------------------------------------------------===//
// Compliant Cases
//===----------------------------------------------------------------------===//

void test_normal_float_operations() {
    float f1 = 3.14f;
    double d1 = 2.718;

    // Normal floating-point operations
    float f2 = f1 + 1.0f;
    double d2 = d1 * 2.0;

    // Cast between floating-point types (allowed)
    float f3 = static_cast<float>(d1);
    double d3 = static_cast<double>(f1);

    // Cast integer to float (allowed)
    int i = 42;
    float f4 = static_cast<float>(i);
    double d4 = static_cast<double>(i);

    (void)f2; (void)d2; (void)f3; (void)d3; (void)f4; (void)d4;
}

void test_normal_pointer_operations() {
    int x = 42;
    int *ptr1 = &x;
    void *vptr;

    // Pointer to pointer casts (allowed)
    vptr = static_cast<void*>(ptr1);
    int *ptr2 = static_cast<int*>(vptr);

    // Normal pointer operations
    int value = *ptr1;
    int *ptr3 = ptr1 + 1;

    (void)ptr2; (void)value; (void)ptr3;
}

void test_float_to_integer_cast() {
    float f = 3.14f;
    double d = 2.718;

    // Cast float to integer (allowed, not pointer-related)
    int i1 = static_cast<int>(f);
    long l1 = static_cast<long>(d);
    unsigned u1 = static_cast<unsigned>(f);

    (void)i1; (void)l1; (void)u1;
}

void test_integer_to_pointer_cast() {
    // Integer to pointer casts (different rule - Rule 11.4)
    // These should NOT be flagged by this check
    long i = 0x1000;
    int *ptr = reinterpret_cast<int*>(i);

    (void)ptr;
}

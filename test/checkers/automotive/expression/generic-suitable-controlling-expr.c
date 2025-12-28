// XFAIL: *
// Test file for: automotive-generic-suitable-controlling-expr
// The controlling expression of a generic selection shall be suitable
//
// RUN: %check_clang_tidy %s automotive-generic-suitable-controlling-expr %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_complete_object_types(void) {
    int x = 5;
    float y = 3.14f;
    double z = 2.71;

    // Compliant: complete object types
    int result1 = _Generic(x, int: 1, float: 2, default: 0);
    int result2 = _Generic(y, int: 1, float: 2, default: 0);
    int result3 = _Generic(z, int: 1, double: 3, default: 0);

    (void)result1;
    (void)result2;
    (void)result3;
}

struct Point {
    int x;
    int y;
};

void test_complete_struct(void) {
    struct Point p = {1, 2};

    // Compliant: complete struct type
    int result = _Generic(p, struct Point: 1, default: 0);

    (void)result;
}

void test_pointer_types(void) {
    int *p = 0;
    void (*func_ptr)(void) = 0;

    // Compliant: pointer types are complete
    int result1 = _Generic(p, int*: 1, float*: 2, default: 0);

    // Compliant: pointer-to-function is valid
    int result2 = _Generic(func_ptr, void(*)(void): 1, default: 0);

    (void)result1;
    (void)result2;
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_void_type(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: controlling expression has void type; use an expression with a complete object type [automotive-generic-suitable-controlling-expr]
    int result = _Generic((void)0, default: 1);

    (void)result;
}

void func_example(void) {
    // Empty function for testing
}

void test_function_type(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: controlling expression has function type 'void (void)'; use pointer-to-function type instead [automotive-generic-suitable-controlling-expr]
    int result = _Generic(func_example, default: 1);

    (void)result;
}

extern int incomplete_array[];

void test_incomplete_array(void) {
    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: controlling expression has incomplete type 'int[]'; use a complete type [automotive-generic-suitable-controlling-expr]
    int result = _Generic(incomplete_array, default: 1);

    (void)result;
}

struct ForwardDeclared;

void test_incomplete_struct(struct ForwardDeclared *p) {
    // Note: *p would be incomplete, but we can't dereference an incomplete type
    // This test shows the concept
    (void)p;
}

void test_vla(int n) {
    // Variable length array - variably modified type
    int vla[n];

    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: controlling expression has variably modified type 'int[n]'; use a type with constant size [automotive-generic-suitable-controlling-expr]
    int result = _Generic(vla, default: 1);

    (void)result;
}

void test_vla_pointer(int n) {
    int vla[n];
    int (*p)[n] = &vla;

    // CHECK-MESSAGES: :[[@LINE+1]]:31: warning: controlling expression has variably modified type 'int (*)[n]'; use a type with constant size [automotive-generic-suitable-controlling-expr]
    int result = _Generic(p, default: 1);

    (void)result;
}

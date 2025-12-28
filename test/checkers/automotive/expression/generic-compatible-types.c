// Test file for: automotive-generic-compatible-types
// Generic association types shall not be compatible
//
// XFAIL: *
// Note: This check is redundant - Clang already errors on compatible types in _Generic
// RUN: %check_clang_tidy %s automotive-generic-compatible-types %t -- -- -std=c11

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_different_types(void) {
    int x = 5;

    // Compliant: int, float, double are not compatible
    int result = _Generic(x,
        int: 1,
        float: 2,
        double: 3);

    (void)result;
}

void test_pointer_types(void) {
    int *p = 0;

    // Compliant: int*, float*, char* are not compatible
    int result = _Generic(p,
        int*: 1,
        float*: 2,
        char*: 3);

    (void)result;
}

void test_with_default(void) {
    long x = 100L;

    // Compliant: types are distinct, default is allowed
    int result = _Generic(x,
        char: 1,
        short: 2,
        int: 3,
        long: 4,
        default: 0);

    (void)result;
}

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

typedef int MyInt;

void test_typedef_same_type(void) {
    int x = 5;

    // CHECK-MESSAGES: :[[@LINE+2]]:9: warning: generic association type is compatible with a previous association type [automotive-generic-compatible-types]
    // CHECK-MESSAGES: :[[@LINE+2]]:9: note: previous association type here
    int result = _Generic(x,
        int: 1,
        MyInt: 2);  // MyInt is typedef for int - compatible!

    (void)result;
}

typedef int Integer;
typedef Integer Num;

void test_chained_typedef(void) {
    int x = 10;

    // CHECK-MESSAGES: :[[@LINE+2]]:9: warning: generic association type is compatible with a previous association type [automotive-generic-compatible-types]
    // CHECK-MESSAGES: :[[@LINE+2]]:9: note: previous association type here
    int result = _Generic(x,
        int: 1,
        Num: 2);  // Num -> Integer -> int - all compatible!

    (void)result;
}

typedef const int ConstInt;

void test_qualified_typedef(void) {
    int x = 42;

    // Note: In C, typesAreCompatible considers qualified types
    // The behavior depends on the implementation
    int result = _Generic(x,
        int: 1,
        ConstInt: 2);

    (void)result;
}

typedef int IntArray[10];

void test_array_typedef(void) {
    int arr[10];

    // Array types - checking for compatibility
    int result = _Generic(arr,
        int*: 1,
        IntArray: 2,
        default: 0);

    (void)result;
}

void test_pointer_typedef(void) {
    typedef int *IntPtr;
    int *p = 0;

    // CHECK-MESSAGES: :[[@LINE+2]]:9: warning: generic association type is compatible with a previous association type [automotive-generic-compatible-types]
    // CHECK-MESSAGES: :[[@LINE+2]]:9: note: previous association type here
    int result = _Generic(p,
        int*: 1,
        IntPtr: 2);  // IntPtr is typedef for int* - compatible!

    (void)result;
}

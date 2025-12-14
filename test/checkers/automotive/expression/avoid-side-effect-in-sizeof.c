// Test file for: automotive-avoid-side-effect-in-sizeof
// Related MISRA C:2025 Rule: 13.6
//
// This file tests the detection of side effects in sizeof operands

// RUN: %check_clang_tidy %s automotive-avoid-side-effect-in-sizeof %t

#include <stddef.h>

//===----------------------------------------------------------------------===//
// Violation Cases (should trigger warnings)
//===----------------------------------------------------------------------===//

void test_increment_decrement_violations(void) {
    int x = 10;
    int arr[20];

    // Increment in sizeof - side effect will not be executed
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains increment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s1 = sizeof(x++);

    // Decrement in sizeof
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains decrement which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s2 = sizeof(x--);

    // Pre-increment in sizeof
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains increment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s3 = sizeof(++x);

    // Pre-decrement in sizeof
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains decrement which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s4 = sizeof(--x);

    // Increment in array expression
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains increment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s5 = sizeof(arr[x++]);

    // Decrement in pointer dereference
    int *ptr = arr;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains increment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s6 = sizeof(*ptr++);
}

void test_assignment_violations(void) {
    int x = 10;
    int y = 20;

    // Simple assignment in sizeof
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s1 = sizeof(x = 42);

    // Assignment from another variable
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s2 = sizeof(x = y);

    // Compound assignment
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains compound assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s3 = sizeof(x += 10);

    // Compound assignment with subtraction
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains compound assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s4 = sizeof(x -= 5);

    // Compound assignment with multiplication
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains compound assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s5 = sizeof(x *= 2);

    // Compound assignment with division
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains compound assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s6 = sizeof(x /= 2);

    // Bitwise compound assignments
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains compound assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s7 = sizeof(x &= 0xFF);

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains compound assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s8 = sizeof(x |= 0x01);

    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains compound assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s9 = sizeof(x ^= 0x55);
}

int external_function(int x);

void test_function_call_violations(void) {
    int x = 10;

    // Function call in sizeof - may have side effects
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains function call which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s1 = sizeof(external_function(x));

    // Multiple function calls
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains function call which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s2 = sizeof(external_function(external_function(x)));
}

void test_complex_violations(void) {
    int x = 10;
    int y = 20;
    int arr[10];

    // Multiple side effects in expression
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains increment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s1 = sizeof(arr[x++] + y++);

    // Side effect in nested expression
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains assignment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s2 = sizeof((x = 5) + y);

    // Side effect in conditional expression
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: sizeof operand contains increment which has potential side effects; these side effects will not be evaluated [automotive-avoid-side-effect-in-sizeof]
    size_t s3 = sizeof(x > 0 ? x++ : x);
}

//===----------------------------------------------------------------------===//
// Compliant Cases (should NOT trigger warnings)
//===----------------------------------------------------------------------===//

void test_compliant_sizeof(void) {
    int x = 10;
    int y = 20;
    int arr[10];

    // sizeof with type - no side effects possible
    size_t s1 = sizeof(int);
    size_t s2 = sizeof(char);
    size_t s3 = sizeof(long);
    size_t s4 = sizeof(double);
    size_t s5 = sizeof(void*);

    // sizeof with variable - no side effects
    size_t s6 = sizeof(x);
    size_t s7 = sizeof(y);
    size_t s8 = sizeof(arr);

    // sizeof with array element - no side effects
    size_t s9 = sizeof(arr[0]);
    size_t s10 = sizeof(arr[5]);

    // sizeof with expression without side effects
    size_t s11 = sizeof(x + y);
    size_t s12 = sizeof(x * 2);
    size_t s13 = sizeof(x / 3);
    size_t s14 = sizeof(x - y);

    // Comparison expressions - no side effects
    size_t s15 = sizeof(x > y);
    size_t s16 = sizeof(x == y);
    size_t s17 = sizeof(x != y);
}

void test_struct_sizeof(void) {
    struct Point {
        int x;
        int y;
    };

    struct Point p = {10, 20};

    // sizeof with struct type
    size_t s1 = sizeof(struct Point);

    // sizeof with struct variable
    size_t s2 = sizeof(p);

    // sizeof with struct member
    size_t s3 = sizeof(p.x);
    size_t s4 = sizeof(p.y);
}

void test_pointer_sizeof(void) {
    int x = 10;
    int *ptr = &x;

    // sizeof with pointer
    size_t s1 = sizeof(ptr);

    // sizeof with dereferenced pointer - no side effect
    size_t s2 = sizeof(*ptr);

    // sizeof with pointer arithmetic expression - no side effect
    size_t s3 = sizeof(*(ptr + 1));

    // sizeof with address-of operator - no side effect
    size_t s4 = sizeof(&x);
}

void test_bitwise_operations(void) {
    int x = 10;
    int y = 20;

    // Bitwise operations without side effects
    size_t s1 = sizeof(x & y);
    size_t s2 = sizeof(x | y);
    size_t s3 = sizeof(x ^ y);
    size_t s4 = sizeof(x << 2);
    size_t s5 = sizeof(x >> 1);
    size_t s6 = sizeof(~x);
}

//===----------------------------------------------------------------------===//
// Edge Cases
//===----------------------------------------------------------------------===//

void test_edge_cases(void) {
    int x = 10;

    // sizeof of sizeof - no side effects
    size_t s1 = sizeof(sizeof(x));

    // Cast without side effects
    size_t s2 = sizeof((float)x);
    size_t s3 = sizeof((double)x);

    // Ternary without side effects
    int y = 20;
    size_t s4 = sizeof(x > 0 ? x : y);

    // Comma operator without side effects (unusual but valid)
    size_t s5 = sizeof((x, y));

    // Parenthesized expression
    size_t s6 = sizeof((x + y));
}

void test_const_expressions(void) {
    // Constant expressions - no side effects
    size_t s1 = sizeof(42);
    size_t s2 = sizeof(3.14);
    size_t s3 = sizeof("Hello");
    size_t s4 = sizeof('A');

    // Constant arithmetic
    size_t s5 = sizeof(10 + 20);
    size_t s6 = sizeof(100 * 2);
}

void test_array_sizeof(void) {
    int arr[10];

    // sizeof entire array
    size_t s1 = sizeof(arr);

    // sizeof array element type
    size_t s2 = sizeof(arr[0]);

    // Calculate number of elements (common pattern)
    size_t num_elements = sizeof(arr) / sizeof(arr[0]);

    // Multi-dimensional array
    int matrix[5][10];
    size_t s3 = sizeof(matrix);
    size_t s4 = sizeof(matrix[0]);
}

void test_typedef_sizeof(void) {
    typedef int IntArray[10];
    typedef struct {
        int x;
        int y;
    } Point;

    // sizeof with typedef'd types
    size_t s1 = sizeof(IntArray);
    size_t s2 = sizeof(Point);

    IntArray arr;
    Point p;

    size_t s3 = sizeof(arr);
    size_t s4 = sizeof(p);
}

// MISRA C++:2023 Rule 6.0.4 (Required)
// The unary minus operator shall not be applied to an expression whose
// underlying type is unsigned.
//
// This file demonstrates VIOLATION examples.

#include <cstdint>

// Violation: Applying unary minus to unsigned int
void violation_unsigned_int() {
    unsigned int x = 5;
    int y = -x;  // Violation: unary minus on unsigned
                 // Result: y = -5 but x wraps to 4294967291 (on 32-bit)
}

// Violation: Applying unary minus to unsigned types in expressions
void violation_in_expression() {
    unsigned int a = 10;
    unsigned int b = 20;
    int result = -a + b;  // Violation: unary minus on unsigned 'a'
    int result2 = a + -b;  // Violation: unary minus on unsigned 'b'
}

// Violation: Unary minus on various unsigned types
void violation_various_unsigned_types() {
    unsigned char uc = 5;
    int y1 = -uc;  // Violation: unary minus on unsigned char

    unsigned short us = 10;
    int y2 = -us;  // Violation: unary minus on unsigned short

    unsigned long ul = 100UL;
    long y3 = -ul;  // Violation: unary minus on unsigned long

    unsigned long long ull = 1000ULL;
    long long y4 = -ull;  // Violation: unary minus on unsigned long long

    uint32_t u32 = 42;
    int32_t y5 = -u32;  // Violation: unary minus on uint32_t

    size_t sz = 100;
    int y6 = -sz;  // Violation: unary minus on size_t (usually unsigned)
}

// Violation: Unary minus with const/volatile qualifiers
void violation_with_qualifiers() {
    const unsigned int cu = 5;
    int y1 = -cu;  // Violation: const doesn't change that it's unsigned

    volatile unsigned int vu = 10;
    int y2 = -vu;  // Violation: volatile doesn't change that it's unsigned
}

// Violation: Unary minus in function calls
void process(int value) {}

void violation_in_function_call() {
    unsigned int x = 5;
    process(-x);  // Violation: unary minus on unsigned
}

// Violation: Unary minus in return statement
int violation_in_return() {
    unsigned int x = 5;
    return -x;  // Violation: unary minus on unsigned
}

// Violation: Common mistake with array indexing
void violation_array_indexing() {
    int arr[10];
    unsigned int idx = 2;
    int val = arr[-idx];  // Violation: dangerous! Negative index from unsigned
}

// Violation: Auto type deduction doesn't help
void violation_with_auto() {
    unsigned int x = 10;
    auto neg = -x;  // Violation: even with auto, unary minus on unsigned
}

// Violation: Parentheses don't change the violation
void violation_with_parentheses() {
    unsigned int x = 5;
    int y = -(x);  // Violation: parentheses don't help
    int z = -((x));  // Violation: still unary minus on unsigned
}

// Rationale for the rule:
// Applying unary minus to unsigned types causes wrap-around behavior:
// -1u = 4294967295 (on 32-bit)
// -5u = 4294967291 (on 32-bit)
// While this is well-defined, it's rarely the intended behavior and
// often indicates a logic error or type mismatch.

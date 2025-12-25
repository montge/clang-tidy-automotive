// MISRA C++:2023 Rule 6.0.4 (Required)
// The unary minus operator shall not be applied to an expression whose
// underlying type is unsigned.
//
// This file demonstrates COMPLIANT code examples.

#include <cstdint>

// Compliant: Using unary minus on signed types
void compliant_signed_types() {
    int x = 5;
    int y = -x;  // OK - signed type

    signed int a = 10;
    signed int b = -a;  // OK - signed type

    long l = 100L;
    long neg_l = -l;  // OK - signed type

    int16_t s = 42;
    int16_t neg_s = -s;  // OK - signed type
}

// Compliant: Explicit cast to signed before negation
void compliant_explicit_cast() {
    unsigned int u = 5;
    int y = -static_cast<int>(u);  // OK - explicit cast to signed first

    unsigned long ul = 100UL;
    long neg_ul = -static_cast<long>(ul);  // OK - explicit cast first
}

// Compliant: Using subtraction from zero instead of unary minus
void compliant_subtraction() {
    unsigned int x = 5;
    int y = 0 - static_cast<int>(x);  // OK - binary subtraction with cast
}

// Compliant: Binary subtraction (not unary minus)
void compliant_binary_subtraction() {
    unsigned int x = 10;
    unsigned int y = 5;
    unsigned int result = x - y;  // OK - binary subtraction
}

// Compliant: Floating point negation
void compliant_floating_point() {
    float f = 5.0f;
    float neg_f = -f;  // OK - floating point

    double d = 10.0;
    double neg_d = -d;  // OK - floating point
}

// Compliant: Negating signed value before assignment to unsigned
void compliant_negate_signed() {
    int x = 5;
    unsigned int y = -x;  // OK - negating a signed value
                          // (though the result assigned to unsigned may wrap)
}

// Compliant: Using literal values
void compliant_literals() {
    int x = -5;  // OK - literal constant
    unsigned int y = -10;  // OK - literal constant (not unary on variable)
}

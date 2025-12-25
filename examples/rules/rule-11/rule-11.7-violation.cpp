// MISRA C:2025 Rule 11.7 Violation Example
// A cast shall not be performed between pointer to object and a non-integer arithmetic type.

#include <cstdint>

void rule_11_7_violations() {
    int x = 42;
    int *ptr = &x;
    void *vptr = &x;

    // Violation: Cast pointer to float
    // Such conversions have no meaningful interpretation
    float f1 = reinterpret_cast<float&>(ptr);

    // Violation: Cast pointer to double
    double d1 = reinterpret_cast<double&>(ptr);

    // Violation: Cast void pointer to float
    float f2 = reinterpret_cast<float&>(vptr);

    // Violation: Cast float to pointer
    float f3 = 3.14f;
    int *p1 = reinterpret_cast<int*&>(f3);

    // Violation: Cast double to pointer
    double d2 = 2.718;
    void *p2 = reinterpret_cast<void*&>(d2);

    // Violation: Round-trip conversion (still violates the rule)
    float temp = reinterpret_cast<float&>(ptr);
    int *restored = reinterpret_cast<int*&>(temp);

    (void)f1; (void)d1; (void)f2; (void)p1; (void)p2; (void)restored;
}

// Note: This rule is Required in MISRA C:2025
// Rationale: Conversions between pointers and floating-point types are
// undefined and serve no practical purpose in conforming C code.

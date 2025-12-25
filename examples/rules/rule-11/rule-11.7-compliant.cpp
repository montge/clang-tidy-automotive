// MISRA C:2025 Rule 11.7 Compliant Example
// A cast shall not be performed between pointer to object and a non-integer arithmetic type.

#include <cstdint>

void rule_11_7_compliant() {
    int x = 42;
    int *ptr = &x;

    // Compliant: Cast between pointer types
    void *vptr = static_cast<void*>(ptr);
    int *ptr2 = static_cast<int*>(vptr);

    // Compliant: Cast between integer types
    long long_val = static_cast<long>(x);
    int int_val = static_cast<int>(long_val);

    // Compliant: Cast between floating-point types
    float f = 3.14f;
    double d = static_cast<double>(f);
    float f2 = static_cast<float>(d);

    // Compliant: Cast integer to float
    float f3 = static_cast<float>(x);
    double d2 = static_cast<double>(x);

    // Compliant: Cast float to integer
    int i1 = static_cast<int>(f);
    long l1 = static_cast<long>(d);

    // Compliant: Pointer arithmetic (different from pointer-to-float cast)
    int arr[10];
    int *p = arr;
    p = p + 5;  // Standard pointer arithmetic

    // Compliant: Cast pointer to integer type (covered by Rule 11.4, not 11.7)
    // Note: This is Advisory in Rule 11.4, but allowed by Rule 11.7
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    int *ptr3 = reinterpret_cast<int*>(addr);

    (void)ptr2; (void)int_val; (void)f2; (void)f3; (void)d2;
    (void)i1; (void)l1; (void)p; (void)ptr3;
}

// Note: Rule 11.7 specifically prohibits casts between pointers and
// non-integer arithmetic types (float, double, long double).
// Casts between pointers and integer types are covered by Rule 11.4.

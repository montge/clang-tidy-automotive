// MISRA C:2025 Rule 23.1 - Generic selection controlling expression
// Category: Required
// Status: COMPLIANT
//
// Generic selections use simple expressions without side effects.

#include <stdint.h>

#define TYPE_NAME(x) _Generic((x), \
    int: "int", \
    float: "float", \
    double: "double", \
    default: "unknown")

// COMPLIANT: Simple variable in _Generic
void simple_example(void) {
    int value = 42;
    const char *name = TYPE_NAME(value);  // No side effect
    (void)name;
}

// COMPLIANT: Evaluate first, then use _Generic
int global_counter = 0;

int increment_and_return(void) {
    return ++global_counter;
}

void separated_example(void) {
    int result = increment_and_return();  // Side effect here
    const char *name = TYPE_NAME(result); // No side effect
    (void)name;
}

// COMPLIANT: Using pointer without modification
void pointer_example(int *ptr) {
    const char *name = TYPE_NAME(*ptr);  // Dereference is not a side effect
    (void)name;
}

// COMPLIANT: Using literal
void literal_example(void) {
    const char *name = TYPE_NAME(3.14f);  // Literal has no side effect
    (void)name;
}

// COMPLIANT: Type-generic math example
#define ABS_VALUE(x) _Generic((x), \
    int: abs_int, \
    float: abs_float, \
    double: abs_double)(x)

int abs_int(int x) { return (x < 0) ? -x : x; }
float abs_float(float x) { return (x < 0.0f) ? -x : x; }
double abs_double(double x) { return (x < 0.0) ? -x : x; }

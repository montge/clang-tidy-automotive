// MISRA C:2025 Rule 23.1 - Generic selection controlling expression
// Category: Required
// Status: VIOLATION
//
// A generic selection shall not have an operand that is dependent
// on any side effect.

#include <stdint.h>

// VIOLATION: Side effect in _Generic controlling expression
int global_counter = 0;

int increment_and_return(void) {
    return ++global_counter;
}

#define TYPE_NAME(x) _Generic((x), \
    int: "int", \
    float: "float", \
    default: "unknown")

void side_effect_example(void) {
    // VIOLATION: increment_and_return() has side effect
    const char *name = TYPE_NAME(increment_and_return());
    (void)name;
}

// VIOLATION: Increment operator in _Generic
void increment_example(int *ptr) {
    // ++(*ptr) has side effect
    const char *name = TYPE_NAME(++(*ptr));
    (void)name;
}

// VIOLATION: Assignment in _Generic
void assignment_example(int *val) {
    int x;
    const char *name = TYPE_NAME(x = *val);  // Assignment side effect
    (void)name;
}

// Note: The controlling expression is evaluated at compile time
// for type determination, but side effects could execute unexpectedly.

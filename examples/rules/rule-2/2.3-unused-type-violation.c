// MISRA C:2025 Rule 2.3 - Unused type declarations
// Category: Advisory
// Status: VIOLATION
//
// A project should not contain unused type declarations.
//
// Expected diagnostic:
//   warning: unused type declaration [automotive-unused-type]

#include <stdint.h>

// VIOLATION: Typedef never used
typedef int32_t UnusedInteger;

// VIOLATION: Struct type defined but never instantiated
typedef struct {
    int x;
    int y;
} UnusedPoint;

// VIOLATION: Enum type defined but never used
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ERROR
} UnusedState;

// VIOLATION: Union type defined but never used
typedef union {
    int32_t as_int;
    float as_float;
} UnusedUnion;

// Only this type is actually used
typedef struct {
    int value;
} UsedStruct;

void function_using_type(void) {
    UsedStruct s;
    s.value = 42;
    (void)s;
}

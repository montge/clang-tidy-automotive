// MISRA C:2025 Rule 23.5 - Violation
// A generic selection should not depend on implicit pointer type conversion
//
// This file demonstrates violations of Rule 23.5, where generic selections
// rely on implicit pointer conversions that could lead to unexpected behavior.

#include <stddef.h>

// Violation: Array-to-pointer decay in controlling expression
void example1_array_decay(void) {
  int numbers[10];

  // The array 'numbers' undergoes implicit array-to-pointer decay,
  // matching int* instead of int[10]
  int result = _Generic(numbers,    // Violation: implicit conversion
                       int*: 1,
                       default: 0);
}

// Violation: Function-to-pointer decay
int compute(int x) {
  return x * 2;
}

void example2_function_decay(void) {
  // The function 'compute' undergoes implicit function-to-pointer decay
  int result = _Generic(compute,                // Violation: implicit conversion
                       int(*)(int): 1,
                       default: 0);
}

// Violation: String literal array-to-pointer decay
void example3_string_literal(void) {
  // String literals decay to char*
  int result = _Generic("hello",    // Violation: implicit conversion
                       char*: 1,
                       default: 0);
}

// Violation: Multi-dimensional array decay
void example4_multidim_array(void) {
  int matrix[3][4];

  // The 2D array decays to a pointer to array
  int result1 = _Generic(matrix,    // Violation: implicit conversion
                        int(*)[4]: 1,
                        default: 0);

  // The first row decays to int*
  int result2 = _Generic(matrix[0], // Violation: implicit conversion
                        int*: 1,
                        default: 0);
}

// Violation: Nested expression with implicit conversion
void example5_nested(void) {
  int values[5];

  // Even with parentheses, the array still decays
  int result = _Generic((values),   // Violation: implicit conversion
                       int*: 1,
                       default: 0);
}

// Advisory: This rule helps prevent confusion about which type is actually
// being matched in a generic selection when pointer conversions occur.

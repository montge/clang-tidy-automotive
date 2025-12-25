// MISRA C:2025 Rule 23.5 - Compliant
// A generic selection should not depend on implicit pointer type conversion
//
// This file demonstrates compliant uses of generic selections where no
// implicit pointer conversions affect the controlling expression.

#include <stddef.h>

// Compliant: Using an array element (not the array itself)
void example1_array_element(void) {
  int numbers[10];

  // Using numbers[0] gives us an int value, no conversion needed
  int result = _Generic(numbers[0],  // Compliant: no conversion
                       int: 1,
                       float: 2,
                       default: 0);
}

// Compliant: Using an explicit pointer variable
void example2_explicit_pointer(void) {
  int value = 42;
  int *ptr = &value;

  // ptr is already int*, no conversion occurs
  int result = _Generic(ptr,         // Compliant: explicit pointer type
                       int*: 1,
                       float*: 2,
                       default: 0);
}

// Compliant: Using scalar types
void example3_scalar_types(void) {
  int i = 10;
  float f = 3.14f;
  double d = 2.718;

  // Direct value types, no pointer conversions
  int r1 = _Generic(i,               // Compliant: scalar type
                   int: 1,
                   float: 2,
                   default: 0);

  int r2 = _Generic(f,               // Compliant: scalar type
                   int: 1,
                   float: 2,
                   default: 0);

  int r3 = _Generic(d,               // Compliant: scalar type
                   int: 1,
                   double: 2,
                   default: 0);
}

// Compliant: Using address-of operator for explicit pointer
void example4_address_of(void) {
  int numbers[10];

  // &numbers[0] explicitly creates a pointer, no implicit conversion
  int result = _Generic(&numbers[0], // Compliant: explicit pointer
                       int*: 1,
                       float*: 2,
                       default: 0);
}

// Compliant: Using dereferenced pointer
void example5_dereferenced_pointer(void) {
  int value = 100;
  int *ptr = &value;

  // *ptr gives us an int value, not a pointer
  int result = _Generic(*ptr,        // Compliant: dereferenced value
                       int: 1,
                       float: 2,
                       default: 0);
}

// Compliant: Explicit cast makes intent clear
void example6_explicit_cast(void) {
  int value = 42;
  int *iptr = &value;

  // Explicit cast to void* shows programmer intent
  int result = _Generic((void*)iptr, // Compliant: explicit cast
                       void*: 1,
                       int*: 2,
                       default: 0);
}

// Compliant: Using pointer arithmetic (result is already a pointer)
void example7_pointer_arithmetic(void) {
  int numbers[10];
  int *ptr = numbers;  // This assignment has decay, but...

  // ...ptr + 1 is already int* type, no conversion in generic selection
  int result = _Generic(ptr + 1,     // Compliant: already pointer type
                       int*: 1,
                       default: 0);
}

// Compliant: Type qualifiers don't involve problematic conversions
void example8_cv_qualifiers(void) {
  int value = 5;
  const int *cptr = &value;
  volatile int *vptr = &value;

  // CV-qualifier conversions are not the focus of this rule
  int r1 = _Generic(cptr,            // Compliant: explicit pointer type
                   const int*: 1,
                   int*: 2,
                   default: 0);

  int r2 = _Generic(vptr,            // Compliant: explicit pointer type
                   volatile int*: 1,
                   int*: 2,
                   default: 0);
}

// Note: The key principle is that the controlling expression's type should
// be clear and not rely on implicit conversions that might confuse which
// association will be selected.

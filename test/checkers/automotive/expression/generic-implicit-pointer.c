// RUN: %check_clang_tidy %s automotive-c25-adv-23.5 %t

// Test MISRA C:2025 Rule 23.5
// A generic selection should not depend on implicit pointer type conversion

// Test case 1: Array-to-pointer decay (violation)
void test_array_to_pointer_decay(void) {
  int arr[5];

  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: generic selection should not depend on implicit pointer type conversion in controlling expression [automotive-c25-adv-23.5]
  int x1 = _Generic(arr, int*: 1, default: 0);

  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: generic selection should not depend on implicit pointer type conversion in controlling expression [automotive-c25-adv-23.5]
  int x2 = _Generic(arr, int[5]: 2, int*: 1);
}

// Test case 2: Void pointer conversions (violation)
void test_void_pointer_conversion(void) {
  void *vptr;
  int *iptr;

  // Void* used directly - this is compliant (no conversion happening)
  int y1 = _Generic(vptr, void*: 1, int*: 2);

  // However, if there's a conversion from int* to void* in the controlling expr
  // (though this is harder to trigger naturally in C)
}

// Test case 3: Function-to-pointer decay (violation)
int my_function(int x) {
  return x + 1;
}

void test_function_to_pointer_decay(void) {
  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: generic selection should not depend on implicit pointer type conversion in controlling expression [automotive-c25-adv-23.5]
  int z1 = _Generic(my_function, int(*)(int): 1, default: 0);
}

// Test case 4: Null pointer conversion (not applicable in this context)
// Note: NULL pointer conversions in _Generic are less common since the
// controlling expression type must exactly match an association.
void test_null_pointer(void) {
  // This doesn't trigger because 0 is int type, not a pointer conversion
  int n1 = _Generic(0, void*: 1, int: 2);
}

// Test case 5: Compliant - explicit pointer type without conversion
void test_compliant_explicit_pointer(void) {
  int x = 5;
  int *ptr = &x;

  // Compliant: ptr is already int*, no conversion needed
  int a1 = _Generic(ptr, int*: 1, float*: 2, default: 0);

  // Compliant: using dereferenced pointer (int type)
  int a2 = _Generic(*ptr, int: 1, float: 2, default: 0);
}

// Test case 6: Compliant - direct value type matching
void test_compliant_value_types(void) {
  int i = 10;
  float f = 3.14f;
  double d = 2.718;

  // All compliant: no pointer conversions
  int b1 = _Generic(i, int: 1, float: 2, double: 3, default: 0);
  int b2 = _Generic(f, int: 1, float: 2, double: 3, default: 0);
  int b3 = _Generic(d, int: 1, float: 2, double: 3, default: 0);
}

// Test case 7: Compliant - using array subscript to avoid decay
void test_compliant_array_element(void) {
  int arr[5] = {1, 2, 3, 4, 5};

  // Compliant: arr[0] is an int, not an array or pointer
  int c1 = _Generic(arr[0], int: 1, float: 2, default: 0);

  // Compliant: using address-of operator gives explicit pointer type
  int c2 = _Generic(&arr[0], int*: 1, float*: 2, default: 0);
}

// Test case 8: Complex - nested expressions with conversions
void test_nested_expressions(void) {
  int arr[10];

  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: generic selection should not depend on implicit pointer type conversion in controlling expression [automotive-c25-adv-23.5]
  int d1 = _Generic((arr), int*: 1, default: 0);

  // Compliant: parentheses don't change that this is an int
  int x = 5;
  int d2 = _Generic((x), int: 1, default: 0);
}

// Test case 9: Pointer type conversions between different types
void test_pointer_type_conversions(void) {
  int x = 42;
  int *iptr = &x;

  // Compliant: no conversion, iptr is already int*
  int e1 = _Generic(iptr, int*: 1, char*: 2, default: 0);

  // Compliant: explicit cast is visible and intentional
  int e2 = _Generic((void*)iptr, void*: 1, int*: 2, default: 0);
}

// Test case 10: Multi-dimensional arrays
void test_multidimensional_array(void) {
  int matrix[3][4];

  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: generic selection should not depend on implicit pointer type conversion in controlling expression [automotive-c25-adv-23.5]
  int f1 = _Generic(matrix, int(*)[4]: 1, default: 0);

  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: generic selection should not depend on implicit pointer type conversion in controlling expression [automotive-c25-adv-23.5]
  int f2 = _Generic(matrix[0], int*: 1, default: 0);
}

// Test case 11: String literals (array-to-pointer decay)
void test_string_literal(void) {
  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: generic selection should not depend on implicit pointer type conversion in controlling expression [automotive-c25-adv-23.5]
  int g1 = _Generic("hello", char*: 1, default: 0);
}

// Test case 12: Typedef pointers (compliant - no conversion)
typedef int* IntPtr;

void test_typedef_pointer(void) {
  int x = 100;
  IntPtr ptr = &x;

  // Compliant: ptr is already a pointer type, no conversion
  int h1 = _Generic(ptr, int*: 1, float*: 2, default: 0);
}

// Test case 13: Const and volatile qualifiers
void test_cv_qualifiers(void) {
  int x = 5;
  const int *cptr = &x;
  volatile int *vptr = &x;

  // Compliant: qualification conversions are typically acceptable
  // (the rule focuses on pointer type conversions, not cv-qualifiers)
  int j1 = _Generic(cptr, const int*: 1, int*: 2, default: 0);
  int j2 = _Generic(vptr, volatile int*: 1, int*: 2, default: 0);
}

// Test case 14: Pointer arithmetic results
void test_pointer_arithmetic(void) {
  int arr[5];
  int *ptr = arr; // This assignment itself has array-to-pointer decay

  // Compliant: ptr+1 is already an int* expression
  int k1 = _Generic(ptr + 1, int*: 1, default: 0);
}

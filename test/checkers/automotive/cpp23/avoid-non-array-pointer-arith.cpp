// RUN: %check_clang_tidy %s automotive-cpp23-req-8.7 %t

// MISRA C++:2023 Rule 8.7 (Partial)
// Pointer arithmetic shall not be applied to a pointer to object of non-array type.

void test_address_of_non_array() {
  int x = 42;
  int *p1 = &x + 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: pointer arithmetic applied to address of non-array variable 'x' [automotive-cpp23-req-8.7]
  // CHECK-MESSAGES: :[[@LINE-3]]:7: note: variable declared here

  int *p2 = &x - 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: pointer arithmetic applied to address of non-array variable 'x' [automotive-cpp23-req-8.7]
  // CHECK-MESSAGES: :[[@LINE-4]]:7: note: variable declared here

  int *p3 = 1 + &x;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: pointer arithmetic applied to address of non-array variable 'x' [automotive-cpp23-req-8.7]
  // CHECK-MESSAGES: :[[@LINE-7]]:7: note: variable declared here

  // Note: Once assigned to a variable, we can't track if it points to an array
  int *p4 = &x;
  p4 += 1;  // OK: we don't know if p4 points to an array or not
}

void test_new_single_object() {
  int *p1 = new int + 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: pointer arithmetic applied to single object allocated with 'new' [automotive-cpp23-req-8.7]
  // CHECK-MESSAGES: :[[@LINE-2]]:13: note: single object allocated here

  int *p2 = new int - 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: pointer arithmetic applied to single object allocated with 'new' [automotive-cpp23-req-8.7]
  // CHECK-MESSAGES: :[[@LINE-2]]:13: note: single object allocated here

  int *p3 = 1 + new int;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: pointer arithmetic applied to single object allocated with 'new' [automotive-cpp23-req-8.7]
  // CHECK-MESSAGES: :[[@LINE-2]]:17: note: single object allocated here
}

void test_increment_decrement() {
  int y = 10;
  int *q1 = &y;
  int *q2 = &y;
  int *q3 = &y;

  ++q1;  // OK: we don't know if q1 points to an array or not
  --q2;  // OK: we don't know if q2 points to an array or not
  q3++;  // OK: we don't know if q3 points to an array or not

  // Note: Direct increment of &y like ++(&y) is a compiler error (rvalue)
  // so we don't test that case
}

void test_array_subscript() {
  int z = 20;
  // Note: Array subscript (&z)[1] is equivalent to *(&z + 1)
  // The compiler transforms it to pointer arithmetic, which is already caught
  // by our binary operator matcher if written explicitly as (&z + 1)
  // However, the subscript form may not trigger our matcher due to AST structure
  int *pz = &z;
  int val = pz[1];  // OK: we don't know if pz points to an array
}

// Compliant: Array types - these should NOT trigger warnings
void test_compliant_array() {
  int arr[10];
  int *p1 = arr + 1;     // OK: arr is an array
  int *p2 = &arr[0] + 1; // OK: element of array
  int *p3 = arr;
  p3 += 2;               // OK: pointer from array

  int val = arr[5];      // OK: normal array indexing
}

// Compliant: Dynamic array allocation
void test_compliant_new_array() {
  int *arr = new int[10];
  int *p = arr + 1;      // OK: new int[10] is an array
  p += 5;                // OK: pointer from array allocation

  delete[] arr;
}

// Compliant: Pointer parameters (we can't determine if they point to arrays)
void test_pointer_param(int *ptr) {
  int *p = ptr + 1;      // OK: we don't know if ptr points to an array
  ptr += 2;              // OK: we don't know if ptr points to an array
  int val = ptr[3];      // OK: we don't know if ptr points to an array
}

// Edge case: Struct member
struct MyStruct {
  int value;
};

void test_struct() {
  MyStruct s;
  MyStruct *p = &s + 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:20: warning: pointer arithmetic applied to address of non-array variable 's' [automotive-cpp23-req-8.7]
  // CHECK-MESSAGES: :[[@LINE-3]]:12: note: variable declared here
}

// Edge case: Multiple indirection
void test_pointer_to_pointer() {
  int x = 5;
  int *px = &x;
  int **ppx = &px;
  int **result = &px + 1;
  // CHECK-MESSAGES: :[[@LINE-1]]:21: warning: pointer arithmetic applied to address of non-array variable 'px' [automotive-cpp23-req-8.7]
  // CHECK-MESSAGES: :[[@LINE-4]]:8: note: variable declared here
}

// Compliant: Standard library containers (std::vector, etc. are not arrays)
#include <vector>

void test_vector() {
  std::vector<int> vec(10);
  // std::vector is not an array type, but this check focuses on obvious
  // violations with primitive types and simple new expressions
  // Container iterators are compliant
}

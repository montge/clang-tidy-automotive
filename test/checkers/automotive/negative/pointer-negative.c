// RUN: clang-tidy %s --checks='-*,automotive-c23-req-11.2,automotive-c23-req-11.8,automotive-wrong-null-pointer-value' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger pointer warnings.

#include <stddef.h>

// Proper pointer initialization with NULL
void pointer_init(void) {
  int *p = NULL;
  void *vp = NULL;
  const int *cp = NULL;
  (void)p;
  (void)vp;
  (void)cp;
}

// Proper pointer assignment
void pointer_assign(void) {
  int x = 10;
  int *p = &x;
  int *q = p;  // Same type assignment - compliant
  (void)q;
}

// Const-correct pointer operations
void const_correct(const int *cp) {
  int x = *cp;  // Reading through const pointer - compliant
  (void)x;
}

// Pointer to pointer (matching types)
void pointer_to_pointer(void) {
  int x = 10;
  int *p = &x;
  int **pp = &p;
  (void)pp;
}

// Void pointer conversions (explicit and allowed in C)
void void_pointer_usage(void) {
  int x = 10;
  void *vp = &x;
  int *ip = vp;  // Implicit void* to int* is allowed in C
  (void)ip;
}

// Proper function pointer usage
typedef int (*func_ptr_t)(int, int);

int add(int a, int b) { return a + b; }

void function_pointer(void) {
  func_ptr_t fp = add;
  int result = fp(1, 2);
  (void)result;
}

// CHECK-NOT: warning:

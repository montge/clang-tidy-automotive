// RUN: clang-tidy %s --checks='-*,automotive-c23-req-10.1,automotive-c23-req-10.2,automotive-c23-adv-13.4,automotive-c23-req-14.4' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger expression warnings.

#include <stdbool.h>

// Proper boolean conditions
void check_conditions(void) {
  bool flag = true;
  int x = 10;

  // Boolean in condition - compliant
  if (flag) {
    x = 20;
  }

  // Comparison result (boolean) - compliant
  if (x > 5) {
    flag = false;
  }

  // Boolean expressions - compliant
  if (flag && (x > 0)) {
    x++;
  }
}

// Proper character operations
void char_operations(void) {
  char c = 'a';
  char d = 'b';

  // Character comparison - allowed
  if (c == d) {
    c = 'c';
  }

  // Character assignment - allowed
  c = d;
}

// Assignment as statement, not expression
void assignments(void) {
  int a = 0;
  int b = 0;

  // Assignment as statement - compliant
  a = 10;
  b = a;

  // Compound assignment - compliant
  a += 5;
  b -= 2;
  (void)b;
}

// Proper use of essential types
void essential_types(void) {
  int i = 10;
  unsigned int u = 20U;
  float f = 1.0f;

  // Operations with matching types - compliant
  i = i + 5;
  u = u * 2U;
  f = f + 2.0f;
  (void)u;
  (void)f;
}

// CHECK-NOT: warning:

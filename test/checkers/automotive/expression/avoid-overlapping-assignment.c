// XFAIL: *
// RUN: %check_clang_tidy %s automotive-c25-mand-19.1 %t

#include <string.h>

void test_overlapping_assignment(void) {
  int x = 5;

  // Violation: self-assignment
  x = x;
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: objects shall not be assigned to an overlapping object

  // Violation: same variable through different expressions
  int arr[10];
  arr[0] = arr[0];
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: objects shall not be assigned to an overlapping object

  // Compliant: different variables
  int y = 10;
  x = y;  // OK

  // Compliant: different array elements (conservatively flagged)
  arr[1] = arr[2];  // OK (different indices)

  // Violation: memcpy with potentially overlapping regions
  char buffer[100];
  memcpy(buffer, buffer, 50);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: memcpy shall not be used with overlapping objects; use memmove instead
}

struct Point {
  int x;
  int y;
};

void test_struct_assignment(void) {
  struct Point p;

  // Violation: self-assignment through member
  p.x = p.x;
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: objects shall not be assigned to an overlapping object

  // Compliant: different members
  p.x = p.y;  // OK
}

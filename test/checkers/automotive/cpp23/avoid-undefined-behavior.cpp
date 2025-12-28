// RUN: %check_clang_tidy %s automotive-cpp23-req-0.3 %t

// MISRA C++:2023 Rule 0.3 - Statically-detectable undefined behavior shall not occur

void testDivisionByZero() {
  int a = 10;
  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: division by zero is undefined behavior (MISRA C++:2023 Rule 0.3)
  int b = a / 0;

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: division by zero is undefined behavior (MISRA C++:2023 Rule 0.3)
  int c = a % 0;

  // Compliant - non-zero divisor
  int d = a / 2;
  int e = a % 3;
}

void testShiftOperations() {
  int x = 1;

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: shift amount >= width of type is undefined behavior (MISRA C++:2023 Rule 0.3)
  int y = x << 32;

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: shift amount >= width of type is undefined behavior (MISRA C++:2023 Rule 0.3)
  int z = x >> 33;

  // CHECK-MESSAGES: :[[@LINE+1]]:13: warning: shift by negative amount is undefined behavior (MISRA C++:2023 Rule 0.3)
  int w = x << -1;

  // Compliant - valid shift amounts
  int a = x << 5;
  int b = x >> 10;
}

void testFloatingPointDivisionByZero() {
  double a = 3.14;

  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: division by zero is undefined behavior (MISRA C++:2023 Rule 0.3)
  double b = a / 0.0;

  // Compliant
  double c = a / 2.0;
}

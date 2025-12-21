// RUN: clang-tidy %s --checks='-*,automotive-avoid-comma-operator,automotive-c23-req-12.2' -- -std=c99 2>&1 | FileCheck -allow-empty %s

// This test verifies that compliant code does not trigger operator warnings.

// Proper shift operations within bounds
void shift_operations(void) {
  unsigned int x = 1U;

  // Shift amount within valid range (0 to width-1)
  unsigned int a = x << 4;   // 4 < 32, valid
  unsigned int b = x >> 8;   // 8 < 32, valid
  unsigned int c = x << 31;  // 31 < 32, valid (max for 32-bit)

  (void)a;
  (void)b;
  (void)c;
}

// Proper for-loop without comma operator
void for_loop_proper(void) {
  int i;
  for (i = 0; i < 10; i++) {
    // loop body
  }
}

// Proper expression sequencing (not comma operator)
void proper_sequencing(void) {
  int a = 1;
  int b = 2;
  int c = 3;

  // Sequential statements, not comma expressions
  a = a + 1;
  b = b + 2;
  c = a + b;
  (void)c;
}

// Function calls with multiple arguments (not comma operator)
int multi_arg_func(int a, int b, int c) {
  return a + b + c;
}

void call_multi_arg(void) {
  int result = multi_arg_func(1, 2, 3);  // Arguments, not comma operator
  (void)result;
}

// CHECK-NOT: warning:

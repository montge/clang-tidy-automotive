// RUN: %check_clang_tidy %s automotive-cpp23-req-16.5 %t

// MISRA C++:2023 Rule 16.5 - A control expression shall be of essentially Boolean type

void testIfStatement() {
  int x = 5;
  int* ptr = &x;

  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: control expression in if statement shall be of essentially Boolean type; found 'int' (MISRA C++:2023 Rule 16.5)
  if (x) {
  }

  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: control expression in if statement shall be of essentially Boolean type; found 'int *' (MISRA C++:2023 Rule 16.5)
  if (ptr) {
  }

  // Compliant - explicit comparison
  if (x != 0) {
  }

  if (x == 5) {
  }

  if (ptr != nullptr) {
  }

  // Compliant - Boolean type
  bool flag = true;
  if (flag) {
  }

  // Compliant - comparison operators
  if (x > 0) {
  }

  if (x <= 10) {
  }
}

void testWhileLoop() {
  int count = 10;

  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: control expression in while statement shall be of essentially Boolean type; found 'int' (MISRA C++:2023 Rule 16.5)
  while (count) {
    count--;
  }

  // Compliant
  while (count > 0) {
    count--;
  }

  bool running = true;
  while (running) {
    running = false;
  }
}

void testDoWhileLoop() {
  int i = 0;

  do {
    i++;
    // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: control expression in do-while statement shall be of essentially Boolean type; found 'int' (MISRA C++:2023 Rule 16.5)
  } while (i);

  // Compliant
  do {
    i++;
  } while (i < 10);
}

void testForLoop() {
  // CHECK-MESSAGES: :[[@LINE+1]]:20: warning: control expression in for statement shall be of essentially Boolean type; found 'int' (MISRA C++:2023 Rule 16.5)
  for (int i = 10; i; i--) {
  }

  // Compliant
  for (int i = 0; i < 10; i++) {
  }

  for (bool done = false; done; ) {
    done = true;
  }
}

void testTernaryOperator() {
  int x = 5;
  int* ptr = &x;

  // CHECK-MESSAGES: :[[@LINE+1]]:16: warning: control expression in ternary operator statement shall be of essentially Boolean type; found 'int' (MISRA C++:2023 Rule 16.5)
  int result = x ? 1 : 0;

  // CHECK-MESSAGES: :[[@LINE+1]]:17: warning: control expression in ternary operator statement shall be of essentially Boolean type; found 'int *' (MISRA C++:2023 Rule 16.5)
  int result2 = ptr ? 10 : 20;

  // Compliant
  int result3 = (x > 0) ? 1 : 0;
  int result4 = (x == 5) ? 1 : 0;

  bool flag = true;
  int result5 = flag ? 1 : 0;
}

void testLogicalOperators() {
  int a = 5, b = 10;

  // Compliant - logical operators produce Boolean results
  if (a > 0 && b > 0) {
  }

  if (a == 5 || b == 10) {
  }

  bool x = true, y = false;
  if (x && y) {
  }

  if (!x) {
  }
}

void testComplexExpressions() {
  int x = 5;

  // Compliant - comparison in complex expression
  if ((x > 0) && (x < 10)) {
  }

  // Compliant - logical not of comparison
  if (!(x == 0)) {
  }
}

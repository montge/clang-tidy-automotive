// RUN: %check_clang_tidy %s automotive-cpp23-mand-7.5.3 %t -- -- -std=c++17
// Test for automotive-cpp23-mand-7.5.3: sizeof operand shall not have side effects

int counter = 0;
int increment() { return counter++; }

void test_sizeof_side_effect() {
  int arr[10];

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: sizeof operand contains increment which has potential side effects; these side effects will not be evaluated [automotive-cpp23-mand-7.5.3]
  int size = sizeof(counter++);

  // CHECK-MESSAGES: :[[@LINE+1]]:10: warning: sizeof operand contains function call which has potential side effects; these side effects will not be evaluated [automotive-cpp23-mand-7.5.3]
  size = sizeof(increment());

  // OK - no side effects
  size = sizeof(arr);
  size = sizeof(int);
  (void)size;
}

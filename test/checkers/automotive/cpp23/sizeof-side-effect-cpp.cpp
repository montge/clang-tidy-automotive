// RUN: %check_clang_tidy %s automotive-cpp23-mand-7.5.3 %t -- -- -std=c++17
// Test for automotive-cpp23-mand-7.5.3: sizeof operand shall not have side effects

int counter = 0;
int increment() { return counter++; }

void test_sizeof_side_effect() {
  int arr[10];
  
  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: side effect in sizeof operand
  int size = sizeof(counter++);
  
  // CHECK-MESSAGES: :[[@LINE+1]]:21: warning: side effect in sizeof operand
  size = sizeof(increment());
  
  // OK - no side effects
  size = sizeof(arr);
  size = sizeof(int);
  (void)size;
}

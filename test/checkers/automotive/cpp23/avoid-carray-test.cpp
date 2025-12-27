// RUN: %check_clang_tidy %s automotive-cpp23-adv-11.3.1 %t -- -- -std=c++17
// Test for automotive-cpp23-adv-11.3.1: C-style arrays shall not be used

void test_local_c_array_violation() {
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: C-style array 'arr' should be replaced with std::array or std::vector
  int arr[10];
  (void)arr;
}

void test_multidimensional_c_array_violation() {
  // CHECK-MESSAGES: :[[@LINE+1]]:7: warning: C-style array 'matrix' should be replaced with std::array or std::vector
  int matrix[3][3];
  (void)matrix;
}

void test_parameter_c_array_compliant(int arr[], int size) {
  // Note: Function parameter arrays decay to pointers
  // The rule typically focuses on declarations, not parameter syntax
  (void)arr;
  (void)size;
}

void test_string_literal_compliant() {
  // String literals are a common exception
  const char *str = "Hello";
  (void)str;
}

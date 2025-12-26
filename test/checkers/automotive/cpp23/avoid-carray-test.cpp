// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-11.3.1 %t -- -- -std=c++17
// Test for automotive-cpp23-req-11.3.1: C-style arrays shall not be used

#include <array>
#include <vector>

void test_local_c_array_violation() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: C-style array declaration
  int arr[10];
  (void)arr;
}

void test_member_c_array_violation() {
  struct S {
    // CHECK-MESSAGES: :[[@LINE+1]]:5: warning: C-style array declaration
    int data[5];
  };
}

void test_multidimensional_c_array_violation() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: C-style array declaration
  int matrix[3][3];
  (void)matrix;
}

void test_std_array_compliant() {
  // OK - using std::array
  std::array<int, 10> arr{};
  (void)arr;
}

void test_std_vector_compliant() {
  // OK - using std::vector
  std::vector<int> vec(10);
  (void)vec;
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

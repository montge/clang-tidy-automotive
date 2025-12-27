// RUN: %check_clang_tidy -std=c++11 %s automotive-cpp23-req-21.6.2 %t
// Test for automotive-cpp23-req-21.6.2: dynamic memory shall be managed automatically

void test_raw_new_delete() {
  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: dynamic memory allocation shall not be used
  int* p = new int;

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: dynamic memory deallocation shall not be used
  delete p;

  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: dynamic memory allocation shall not be used
  int* arr = new int[10];

  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: dynamic memory deallocation shall not be used
  delete[] arr;
}

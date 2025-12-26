// XFAIL: *
// Note: MISRA cpp23 checks not yet implemented
// RUN: %check_clang_tidy %s automotive-cpp23-req-21.6.2 %t -- -- -std=c++17
// Test for automotive-cpp23-req-21.6.2: dynamic memory shall be managed automatically

void test_raw_new_delete() {
  // CHECK-MESSAGES: :[[@LINE+1]]:12: warning: avoid using raw new
  int* p = new int;
  
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: avoid using raw delete
  delete p;
  
  // CHECK-MESSAGES: :[[@LINE+1]]:14: warning: avoid using raw new
  int* arr = new int[10];
  
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: avoid using raw delete
  delete[] arr;
}

#include <memory>

void test_smart_ptr() {
  // OK - managed automatically
  auto p = std::make_unique<int>(42);
  auto sp = std::make_shared<int>(10);
}

// RUN: %check_clang_tidy %s automotive-cpp23-req-10.4.1 %t -- -- -std=c++17
// Test for automotive-cpp23-req-10.4.1: volatile qualifier shall not be used

// CHECK-MESSAGES: :[[@LINE+1]]:1: warning: volatile qualifier shall not be used
volatile int global_var;

void test_volatile() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: volatile qualifier shall not be used
  volatile int local_var = 0;
  (void)local_var;
}

// OK - no volatile
int normal_var = 0;

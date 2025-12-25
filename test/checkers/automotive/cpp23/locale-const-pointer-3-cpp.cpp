// RUN: %check_clang_tidy %s automotive-cpp23-mand-25.5.3 %t -- -- -std=c++17
// Test for automotive-cpp23-mand-25.5.3: strerror pointer shall be const

#include <cstring>

void test_strerror() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: pointer from strerror shall only be used as pointer to const
  char* err = std::strerror(1);
  
  // OK - const pointer
  const char* err_const = std::strerror(2);
  (void)err;
  (void)err_const;
}

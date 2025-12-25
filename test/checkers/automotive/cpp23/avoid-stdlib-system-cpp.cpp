// RUN: %check_clang_tidy %s automotive-cpp23-req-21.2.3 %t -- -- -std=c++17
// Test for automotive-cpp23-req-21.2.3: system() from <cstdlib> shall not be used

#include <cstdlib>

void test_system() {
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: avoid std::system
  std::system("ls");
  
  // CHECK-MESSAGES: :[[@LINE+1]]:3: warning: avoid std::system
  system("pwd");
}
